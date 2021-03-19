/*
 * Copyright (c) 2021, Andreas Kling <kling@serenityos.org>
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

#include "VncClient.h"
#include "VncViewerAppWindow.h"
#include <AK/Debug.h>
#include <AK/QuickSort.h>
#include <AK/StringBuilder.h>
#include <LibCore/DateTime.h>
#include <LibCore/Notifier.h>
#include <pwd.h>
#include <stdio.h>
#include <strings.h>

VncClient::VncClient(String server, int port) :
    m_config(Core::ConfigFile::get_for_app("VncViewer"))
{
    m_socket = Core::TCPSocket::construct(this);

    if (server.is_empty()) {
        m_hostname = m_config->read_entry("Connection", "Server", "");
        m_port = m_config->read_num_entry("Connection", "Port", VNC_PORT);
    } else {
        m_hostname = server;
        m_port = port ? port : VNC_PORT;
    }
}

VncClient::~VncClient()
{
}

void VncClient::set_server(const String& hostname, int port)
{
    m_hostname = hostname;
    m_port = port;
    m_config->write_entry("Connection", "Server", hostname);
    m_config->write_num_entry("Connection", "Port", port);
    m_config->sync();
}

void VncClient::on_socket_connected()
{
    perror("on_socket_connected called!");

    m_notifier = Core::Notifier::construct(m_socket->fd(), Core::Notifier::Read);
    m_notifier->on_ready_to_read = [this] { receive_from_server(); };
}

bool VncClient::connect()
{
    if (m_socket->is_connected())
        VERIFY_NOT_REACHED();

    dbgln("connect() called!");

    m_socket->on_connected = [this] { on_socket_connected(); };

    dbgln("calling socket->connect()!");
    return m_socket->connect(m_hostname, m_port);
}

void VncClient::receive_from_server()
{
    perror("receive_from_server called!");

    while (m_socket->can_read_line()) {
        auto line = m_socket->read_line();
        if (line.is_null()) {
            if (!m_socket->is_connected()) {
                outln("VncClient: Connection closed!");
                exit(1);
            }
            VERIFY_NOT_REACHED();
        }

        auto msg = String::formatted("Got line: {}", line);
        perror(msg.characters());
    }
}

void VncClient::send(const String& text)
{
    if (!m_socket->send(text.bytes())) {
        perror("send");
        exit(1);
    }
}

void VncClient::send(const ReadonlyBytes& bytes)
{
    if (!m_socket->send(bytes)) {
        perror("send");
        exit(1);
    }
}
