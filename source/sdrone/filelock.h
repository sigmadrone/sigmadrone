/*
 * filelock.h
 *
 *  Created on: Dec 5, 2013
 *      Author: svassilev
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
