/*
 * Copyright (c) 2009, 2012, Oracle and/or its affiliates. All rights reserved.
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
package test.output;

import java.lang.ref.*;

/**
 * This class implements a simple test for weak references and reference queues.
 */
public class WeakReferenceTest01 {
    public static void main(String[] args) {
        // test basic weak reference behavior
        final WeakReference<String> w1 = new WeakReference<String>("alive");
        test(w1);
        final WeakReference<String> w2 = new WeakReference<String>(new String("alive"));
        test(w2);

        // now test with a reference queue
        final ReferenceQueue<String> queue = new ReferenceQueue<String>();
        final WeakReference<String> w3 = new WeakReference<String>(new String("alive"), queue);
        test(w3);
    }

    private static void test(final WeakReference<? extends Object> w1) {
        System.out.println("" + w1.get());
        System.gc();
        System.out.println("" + w1.get());
    }
}
