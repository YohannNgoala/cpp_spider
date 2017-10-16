#pragma once

#ifndef STATUS_HPP_
# define STATUS_HPP_

class			Status
{
private:
	bool		_isPaused;
	bool		_isClickActivated;
	bool		_isTrackingActivated;
	bool		_isKbActivated;

public:
	Status() : 
		_isPaused(false), _isClickActivated(false), _isTrackingActivated(false), _isKbActivated(false) {}
	~Status() = default;

	bool		isPaused() { return (_isPaused); }
	bool		isClickActivated() { return (_isClickActivated); }
	bool		isTrackingActivated() { return (_isTrackingActivated); }
	bool		isKbActivated() { return (_isKbActivated); }

	void		setPause(bool pause) { _isPaused = pause; }
	void		setClick(bool click) { _isClickActivated = click; }
	void		setTracking(bool tracking) { _isTrackingActivated = tracking; }
	void		setKb(bool keyBoard) { _isKbActivated = keyBoard; }

	void		activatePause() { _isPaused = true; }
	void		activateClick() { _isClickActivated = true; }
	void		activateTracking() { _isTrackingActivated = true; }
	void		activateKb() { _isKbActivated = true; }

	void		deactivatePause() { _isPaused = false; }
	void		deactivateClick() { _isClickActivated = false; }
	void		deactivateTracking() { _isTrackingActivated = false; }
	void		deactivateKb() { _isKbActivated = false; }
};

#endif // !STATUS_HPP_