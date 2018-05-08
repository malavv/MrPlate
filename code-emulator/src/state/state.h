#ifndef _STATE_STATE_H_
#define _STATE_STATE_H_

enum States : int8_t {
	INIT = 0,
	MENU = 1,
	BASIC = 2,
	ADVAN = 3,
	INFO = 4
};

class State {
public:
	State() {}
	virtual ~State() {}
	virtual States process() = 0;
	virtual State* onEnter() = 0;
	virtual int8_t id() = 0;

	virtual void backPressed() = 0;
	virtual void selectPressed() = 0;
	virtual void navTurned(int8_t delta) = 0;
};

#endif // _STATE_STATE_H_