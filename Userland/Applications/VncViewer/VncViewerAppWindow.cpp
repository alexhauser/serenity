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

#include "VncViewerAppWindow.h"
#include "VncClient.h"
#include <LibGUI/Action.h>
#include <LibGUI/Application.h>
#include <LibGUI/BoxLayout.h>
#include <LibGUI/InputBox.h>
#include <LibGUI/Menu.h>
#include <LibGUI/MenuBar.h>
#include <LibGUI/Splitter.h>
#include <LibGUI/StackWidget.h>
#include <LibGUI/TableView.h>
#include <LibGUI/ToolBar.h>
#include <LibGUI/ToolBarContainer.h>
#include <LibRFB/RFBProto.h>

using namespace RFB;

static VncViewerAppWindow* s_the;

VncViewerAppWindow& VncViewerAppWindow::the()
{
    return *s_the;
}

VncViewerAppWindow::VncViewerAppWindow(String server, int port)
{
    VERIFY(!s_the);
    s_the = this;

    //String value;
    //if (GUI::InputBox::show(this, value, "Enter server:", "Connect to server") == GUI::InputBox::ExecCancel)
    //   ::exit(0);
    //server = value;

    auto client = VncClient::construct("192.168.100.101", 5900);

    if (!client->connect()) {
        perror("Connect failed!");
        return;
    } else {
        dbgln("Connect succeeded!");
    }

    ReadonlyBytes* data;
    do {
        data = client->receive();
    } while (data == nullptr);

    dbgln("Got {} bytes from server: {}", data->size(), data->data());

    client->send("RFB 003.008\n");
    dbgln("Send took place!");


    RFBProto::ProtocolVersion pv;

    String test = String::formatted("Server: {}:{} -> {}", server, port, pv.serialize());
    this->set_title(test);

    set_icon(Gfx::Bitmap::load_from_file("/res/icons/16x16/app-irc-client.png"));

    resize(600, 400);

}

VncViewerAppWindow::~VncViewerAppWindow()
{
}
