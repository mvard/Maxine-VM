/*
 * Copyright (c) 2012, Oracle and/or its affiliates. All rights reserved.
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
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */
package com.sun.max.tele.object;

import com.sun.max.tele.reference.*;

// TODO (mlvdv) articulate behavior with respect to relocated/forwarded objects.
/**
 * The phases of a "region manager" that is responsible for object
 * memory allocation and reclamation for one or more extends of
 * heap memory.  The manager cycles in order through he following three
 * phases:
 * <ol>
 * <li> {@link #ALLOCATING}: normal operation, new memory being allocated on demand.</li>
 * <li> {@link #ANALYZING}: first ("liveness analysis") phase of GC.</li>
 * <li> {@link #RECLAIMING}: final ("clean up") phase of GC.</li>
 * </ol>
 */
public enum ObjectManagerStatus {

    /**
     * This is the non-GC phase, during which the only activity
     * by the manager is to allocate object memory from its free
     * list, which is initially {@link ObjectMemoryStatus#LIVE}.
     * During this phase any {@link ObjectMemoryStatus#LIVE}
     * object in one of the manager's regions remains
     * {@link ObjectMemoryStatus#LIVE}, unmoved, and of constant type.
     */
    ALLOCATING("Allocating", "Allocating only, no GC"),

    /**
     * The first phase of a GC, during which the  manager investigates
     * the liveness of every object in its region without loss of historical
     * information. During this phase no new objects are allocated.  The status
     * of all objects becomes {@link ObjectMemoryStatus#UNKNOWN} at the beginning
     * of this phase and they may or may not become once again {@link ObjectMemoryStatus#LIVE}
     * during the phase. Objects whose status remains {@link ObjectMemoryStatus#UNKNOWN}
     * at the end of the phase are deemed unreachable and become permanently
     * {@link ObjectMemoryStatus#DEAD}.
     */
    ANALYZING("Analyzing", "First (liveness analysis) phase of GC"),

    /**
     * The second phase of a GC, during which the manager finalizes the status of
     * objects and reclaims unreachable memory that had been allocated to objects
     * now determined to be {@link ObjectMemoryStatus#DEAD}, as well as any other
     * information produced during GC that is no longer needed.
     */
    RECLAIMING("Reclaiming", "Second (clean up, reclaim memory) phase of GC");

    private final String label;
    private final String description;

    private ObjectManagerStatus(String label, String description) {
        this.label = label;
        this.description = description;
    }

    public String label() {
        return label;
    }

    public String description() {
        return description;
    }
}