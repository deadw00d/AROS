/*
 * $Id: StartSound.java,v 1.2 2002/02/15 23:44:28 skavish Exp $
 *
 * ===========================================================================
 *
 * The JGenerator Software License, Version 1.0
 *
 * Copyright (c) 2000 Dmitry Skavish (skavish@usa.net). All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution, if
 *    any, must include the following acknowlegement:
 *    "This product includes software developed by Dmitry Skavish
 *     (skavish@usa.net, http://www.flashgap.com/)."
 *    Alternately, this acknowlegement may appear in the software itself,
 *    if and wherever such third-party acknowlegements normally appear.
 *
 * 4. The name "The JGenerator" must not be used to endorse or promote
 *    products derived from this software without prior written permission.
 *    For written permission, please contact skavish@usa.net.
 *
 * 5. Products derived from this software may not be called "The JGenerator"
 *    nor may "The JGenerator" appear in their names without prior written
 *    permission of Dmitry Skavish.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL DMITRY SKAVISH OR THE OTHER
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

package org.openlaszlo.iv.flash.api.sound;

import java.io.PrintStream;
import org.openlaszlo.iv.flash.parser.*;
import org.openlaszlo.iv.flash.util.*;
import org.openlaszlo.iv.flash.api.*;

public class StartSound extends FlashObject {

    protected FlashDef def;
    protected SoundInfo soundInfo;

    public StartSound() {}

    public int getTag() {
        return Tag.STARTSOUND;
    }

    public static StartSound parse( Parser p ) {

        StartSound o = new StartSound();

        o.def = p.getDef( p.getUWord() );

        o.soundInfo = SoundInfo.parse( p );

        return o;

    }

    public static StartSound newStartSound( Sound def, SoundInfo soundInfo ) {

        StartSound startSound = new StartSound();

        startSound.def = def;
        startSound.soundInfo = soundInfo;

        return startSound;

    }

    public void collectDeps( DepsCollector dc ) {
        if( def != null ) dc.addDep(def);
    }

    public void write( FlashOutput fob ) {

        fob.writeTag( getTag(), 2 + soundInfo.length() );

        fob.writeDefID( def );

        soundInfo.write( fob );
    }

    public void printContent( PrintStream out, String indent ) {
        out.println( indent+"StartSound:" );
    }

    public boolean isConstant() {
        return true;
    }

    protected FlashItem copyInto( FlashItem item, ScriptCopier copier ) {
        super.copyInto( item, copier );
        ((StartSound)item).soundInfo = (SoundInfo) soundInfo.getCopy( copier );
        ((StartSound)item).def = copier.copy(def);
        return item;
    }

    public FlashItem getCopy( ScriptCopier copier ) {
        return copyInto( new StartSound(), copier );
    }
}
