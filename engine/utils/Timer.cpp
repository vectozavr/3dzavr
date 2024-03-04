#include <utils/Timer.h>

using namespace std::chrono;

void Timer::start() {
    if(!_isTicking) {
        _startTime = high_resolution_clock::now();
    }
    if(_isStopped) {
        _elapsedSeconds = 0;
    }
    _isStopped = false;
    _isTicking = true;
}

void Timer::pause() {

    _endTime = high_resolution_clock::now();
    _elapsedSeconds += duration<double>(_endTime - _startTime).count();

    _isTicking = false;
}

void Timer::stop() {
    if(_isTicking) {
        _endTime = high_resolution_clock::now();
        _elapsedSeconds += duration<double>(_endTime - _startTime).count();
    }

    _isTicking = false;
    _isStopped = true;
}

double Timer::elapsedSeconds() const {
    if(_isTicking) {
        return _elapsedSeconds + duration<double>(high_resolution_clock::now() - _startTime).count();
    }

    return _elapsedSeconds;
}

double Timer::elapsedMilliseconds() const {
    return elapsedSeconds()*1000;
}
