/* Copyright (C) 2014  Alex-Daniel Jakimenko <alex.jakimenko@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include "notificationManager.hpp"

#include "wakeup.hpp"

namespace notificationManager {

Notification* activeNotification;
std::vector<Notification*> notifications;

void add(Notification* newNotification) {
    if (notifications.size() == 0) // put dummy notification so that we always have something to fall back to
        notifications.push_back(new Notification(0, 0, 0, -10000));
    notifications.push_back(newNotification);
    chooseMostPrioritized();
}

void update() {
    if (activeNotification != nullptr)
        if (!activeNotification->turnedOn || activeNotification->update()) {
            activeNotification->turnOff();
            chooseMostPrioritized();
        }
}

void chooseMostPrioritized() {
    if (activeNotification != nullptr && activeNotification->turnedOn)
        activeNotification->pause();
    activeNotification = nullptr;
    for (auto &curNotification : notifications) // XXX not the most efficient way, but it is good enough for now
        if (curNotification->turnedOn)
            if (activeNotification == nullptr || activeNotification->priority <= curNotification->priority) {
                activeNotification = curNotification;
                wakeup::add(activeNotification->curDuration);
            }
    if (activeNotification != nullptr)
        activeNotification->play();
}

}
