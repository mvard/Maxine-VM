/*
 * Copyright (c) 2017, APT Group, School of Computer Science,
 * The University of Manchester. All rights reserved.
 * Copyright (c) 2007, 2011, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <malloc.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/user.h>
#include <alloca.h>

#include "log.h"
#include "jni.h"
#include "isa.h"
#include "threads.h"
#include "ptrace.h"

#include "teleProcess.h"
#include "teleNativeThread.h"
#include "linuxTask.h"

boolean task_read_registers(pid_t tid,
    isa_CanonicalIntegerRegistersStruct *canonicalIntegerRegisters,
    isa_CanonicalStateRegistersStruct *canonicalStateRegisters,
    isa_CanonicalFloatingPointRegistersStruct *canonicalFloatingPointRegisters) {

#ifdef __riscv
    /* Michalis Vardoulakis @ Fri May 3: As far as I can tell ptrace is not implemented on linux for riscv*/
    return false;
#else
    if (canonicalIntegerRegisters != NULL || canonicalStateRegisters != NULL) {
#ifndef __arm__
        struct user_regs_struct osIntegerRegisters;
#else
	    struct user_regs osIntegerRegisters;
#endif
        if (ptrace(PT_GETREGS, tid, 0, &osIntegerRegisters) != 0) {
            return false;
        }
        if (canonicalIntegerRegisters != NULL) {
            isa_canonicalizeTeleIntegerRegisters(&osIntegerRegisters, canonicalIntegerRegisters);
        }
        if (canonicalStateRegisters != NULL) {
            isa_canonicalizeTeleStateRegisters(&osIntegerRegisters, canonicalStateRegisters);
        }
    }

    if (canonicalFloatingPointRegisters != NULL) {
#ifdef __arm__
        struct user_fpregs osFloatRegisters;
#elif defined __aarch64__
        struct user_fpsimd_struct osFloatRegisters;
#else
        struct user_fpregs_struct osFloatRegisters;
#endif
        if (ptrace(PT_GETFPREGS, tid, 0, &osFloatRegisters) != 0) {
            return false;
        }
        isa_canonicalizeTeleFloatingPointRegisters(&osFloatRegisters, canonicalFloatingPointRegisters);
    }

    return true;
#endif /*__riscv*/
}

ThreadState_t toThreadState(char taskState, pid_t tid) {
    ThreadState_t threadState;
    switch (taskState) {
        case 'W':
        case 'D':
        case 'S': threadState = TS_SLEEPING; break;
        case 'R': threadState = TS_RUNNING; break;
        case 'T': threadState = TS_SUSPENDED; break;
        case 'Z': threadState = TS_DEAD; break;
        default:
            log_println("Unknown task state '%c' for task %d interpreted as thread state TS_DEAD", taskState, tid);
            threadState = TS_DEAD;
            break;
    }
    return threadState;
}

static void gatherThread(JNIEnv *env, pid_t tgid, pid_t tid, jobject linuxTeleProcess, jobject threadList, jlong tlaList) {

    isa_CanonicalIntegerRegistersStruct canonicalIntegerRegisters;
    isa_CanonicalStateRegistersStruct canonicalStateRegisters;

    char taskState = task_state(tgid, tid);

    TLA tla = 0;
    if (taskState == 'T' && task_read_registers(tid, &canonicalIntegerRegisters, &canonicalStateRegisters, NULL)) {
#ifdef __arm__
        Address stackPointer = (Address) canonicalIntegerRegisters.r13;
#elif defined __aarch64__
        Address stackPointer = (Address) canonicalStateRegisters.sp;
#elif defined(__riscv)
        Address stackPointer = (Address) canonicalIntegerRegisters.x2;
#else
        Address stackPointer = (Address) canonicalIntegerRegisters.rsp;
#endif
        TLA threadLocals = (TLA) alloca(tlaSize());
        NativeThreadLocalsStruct nativeThreadLocalsStruct;
        ProcessHandleStruct ph = {tgid, tid};
        tla = teleProcess_findTLA(&ph, tlaList, stackPointer, threadLocals, &nativeThreadLocalsStruct);
    }
#ifdef __riscv
    teleProcess_jniGatherThread(env, linuxTeleProcess, threadList, tid, toThreadState(taskState, tid), (jlong) canonicalIntegerRegisters.x3, tla);
#else
    teleProcess_jniGatherThread(env, linuxTeleProcess, threadList, tid, toThreadState(taskState, tid), (jlong) canonicalStateRegisters.rip, tla);
#endif
}

JNIEXPORT void JNICALL
Java_com_sun_max_tele_debug_linux_LinuxNativeTeleChannelProtocol_nativeGatherThreads(JNIEnv *env, jclass c, jlong pid, jobject linuxTeleProcess, jobject threads, long tlaList) {

    pid_t *tasks;
    const int nTasks = scan_process_tasks(pid, &tasks);
    if (nTasks < 0) {
        log_println("Error scanning /proc/%d/task directory: %s", pid, strerror(errno));
        return;
    }

    int n = 0;
    while (n < nTasks) {
        pid_t tid = tasks[n];
        gatherThread(env, pid, tid, linuxTeleProcess, threads, tlaList);
        n++;
    }
    free(tasks);
}
