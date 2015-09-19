/*
 * HummingDroid, Android QuadCopter Controller
 * Copyright (C) 2013 Cedric Priscal
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _VALUE_H_
#define _VALUE_H_

#include "Communication.pb.h"
#include "Object.h"
#include "Timestamp.h"

namespace org {
namespace hummingdroid {
namespace flightapp {

class Value {
public:
    // Default constructor
    Value();

    // Copy constructor
    Value(const Value & that);

    // Attributes
    float value;
    Timestamp timestamp;

    // Operators
    void operator =(const Value & value) {
        this->value = value.value;
        this->timestamp = value.timestamp;
    }

    void set(const Value & value);
    void set(float value, const Timestamp & timestamp);
    void add(const Value & a, const Value & b);
    void limit(float min, float max);
    void amplify(const Value & value, float gain);
    void amplify(float gain);
    void reset();
};

class Derivator : public Value {
private:
    float prev;

public:
    Derivator();
    void derive(const Value & value);
};

class Integrator : public Value {
private:
    float prev;
public:
    Integrator();
    void integrate(const Value & value);
    void reset();
};

class LowPass : public Value {
protected:
    float T;

public:
    LowPass(float T = 0.);
    void setT(float T);
    void lowpass(const Value &value);
};

class HighPass : public Value {
protected:
    LowPass lowpass;

public:
    void setT(float T);
    void highpass(const Value & value);
};

class PID : public Object, public Value {
private:
    Value propo;
    Integrator integ;
    Value integ_factor;
    LowPass low_pass;
    Derivator deriv;
    Value deriv_factor;
    hummingdroid::PID params;

public:
    void setParams(const hummingdroid::PID & params);
    void pid(Value value);
    void reset();
};

}
}
}

#endif
