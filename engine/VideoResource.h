#pragma once
#include "Common.h"

class VideoResource : public RefCounted {
public:
	virtual ~VideoResource() { }

	///video resources can have an optional name
	const std::string GetName() const  { return m_name; }
	void SetName(const std::string &n) { m_name = n;    }

	///reload resource from file system, if possible
	virtual void Reload() { }

protected:
	std::string m_name;
};