//
// Aspia Project
// Copyright (C) 2019 Dmitry Chapyshev <dmitry@aspia.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#ifndef HOST__HOST_SERVICE_H
#define HOST__HOST_SERVICE_H

#include "qt_base/service.h"

#include <QCoreApplication>

namespace base::win {
class ScopedCOMInitializer;
} // namespace base::win

namespace host {

class Server;

class Service : public qt_base::Service<QCoreApplication>
{
public:
    Service();
    ~Service();

protected:
    // base::Service implementation.
    void start() override;
    void stop() override;
    void sessionEvent(base::win::SessionStatus status, base::win::SessionId session_id) override;

private:
    std::unique_ptr<base::win::ScopedCOMInitializer> com_initializer_;
    std::unique_ptr<Server> server_;

    DISALLOW_COPY_AND_ASSIGN(Service);
};

} // namespace host

#endif // HOST__HOST_SERVICE_H
