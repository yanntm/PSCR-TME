#pragma once


class Job {
public:
	virtual void run () = 0;
	virtual ~Job() {};
};
