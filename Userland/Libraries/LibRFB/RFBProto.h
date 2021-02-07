/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <AK/ByteBuffer.h>
#include <AK/Optional.h>
#include <AK/String.h>
#include <AK/URL.h>
#include <AK/Vector.h>
#include <LibCore/Forward.h>

namespace RFB {

class RFBProto {
public:
    struct ProtocolVersion {
        int major { 3 };
        int minor { 8 };

        String serialize() {
            char buffer[13] { 0 };
            snprintf(buffer, sizeof(buffer), "RFB %03d.%03d\n", major, minor);
            return String(buffer);
        }
    };

    enum SecurityType {
        Invalid = 0,
        None = 1,
        VNCAuth = 2,
        TightSecurity = 16,
        VeNCrypt = 19
    };

    enum ProtocolStage {
        Handshaking,
        Initialisation,
        Normal
    };

    enum Encoding {
        Raw = 0,
        CopyRect = 1,
        RRE = 2,
        CoRRE = 4,
        Hextile = 5,
        zlib = 6,
        Tight = 7,
        zlibhex = 8,
        Ultra = 9,
        Ultra2 = 10,
        TRLE = 15,
        ZRLE = 16,
        HitachiZYWRLE = 17,
        H264 = 20,
        JPEG = 21,
        JRLE = 22,
        TightPNG = -260
    };  

    RFBProto();
    ~RFBProto();

private:
    Encoding m_encoding;
    ProtocolVersion m_protocol_version { 3, 8 };
};

}
