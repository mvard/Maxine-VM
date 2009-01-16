/*
 * Copyright (c) 2007 Sun Microsystems, Inc.  All rights reserved.
 *
 * Sun Microsystems, Inc. has intellectual property rights relating to technology embodied in the product
 * that is described in this document. In particular, and without limitation, these intellectual property
 * rights may include one or more of the U.S. patents listed at http://www.sun.com/patents and one or
 * more additional patents or pending patent applications in the U.S. and in other countries.
 *
 * U.S. Government Rights - Commercial software. Government users are subject to the Sun
 * Microsystems, Inc. standard license agreement and applicable provisions of the FAR and its
 * supplements.
 *
 * Use is subject to license terms. Sun, Sun Microsystems, the Sun logo, Java and Solaris are trademarks or
 * registered trademarks of Sun Microsystems, Inc. in the U.S. and other countries. All SPARC trademarks
 * are used under license and are trademarks or registered trademarks of SPARC International, Inc. in the
 * U.S. and other countries.
 *
 * UNIX is a registered trademark in the U.S. and other countries, exclusively licensed through X/Open
 * Company, Ltd.
 */
package com.sun.max.tele;

import com.sun.max.tele.debug.*;
import com.sun.max.tele.object.*;
import com.sun.max.vm.reference.*;

/**
 * Convenience methods for all local objects that refer to something in a {@link TeleVM}.
 *
 * @author Bernd Mathiske
 * @author Michael Van De Vanter
 */
public abstract class TeleVMHolder {

    private final TeleVM _teleVM;

    public TeleVM teleVM() {
        return _teleVM;
    }

    private final String _tracePrefix;

    /**
     * @return default prefix text for trace messages; identifies the class being traced.
     */
    protected String tracePrefix() {
        return _tracePrefix;
    }

    protected TeleVMHolder(TeleVM teleVM) {
        _teleVM = teleVM;
        _tracePrefix = "[" + getClass().getSimpleName() + "] ";
    }

    /**
     * @return the process associated with this VM.
     */
    public TeleProcess teleProcess() {
        return _teleVM.teleProcess();
    }

    /**
     * @param reference an object in the r{@link TeleVM}
     * @return canonical local surrogate for the remote object.
     */
    public TeleObject makeTeleObject(Reference reference) {
        return _teleVM.makeTeleObject(reference);
    }

}
