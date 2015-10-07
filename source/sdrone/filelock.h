/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
 */

#ifndef FILELOCK_H_
#define FILELOCK_H_

#include <string>
#include <sys/file.h>
#include <sys/stat.h>

struct FileLock
{
	FileLock(const char* name, bool doLock=false) {
		struct stat st = {0};
		if (stat("/var/lock/sigmadrone", &st) == -1) {
		    mkdir("/var/lock/sigmadrone", S_IRUSR|S_IWUSR|S_IXUSR);
		}
		m_name = std::string("/var/lock/sigmadrone/") + std::string(name) +
				std::string(".lock");
		m_file = 0;
		if (doLock) {
			Lock();
		}
	}
	~FileLock() {
		Unlock();
	}
	bool /*locked*/ Lock() {
		if (!IsLocked()) {
			if (0 != (m_file = fopen(m_name.c_str(),"w+"))) {
				if (0 != flock(fileno(m_file),LOCK_EX|LOCK_NB)) {
					fclose(m_file);
					m_file = 0;
				}
			}
		}
		return IsLocked();
	}
	void Unlock() {
		if (IsLocked()) {
			remove(m_name.c_str());
			fclose(m_file);
			m_file=0;
		}
	}
	bool IsLocked() { return !!m_file; }
private:
	FILE* m_file;
	std::string m_name;
};



#endif /* FILELOCK_H_ */
