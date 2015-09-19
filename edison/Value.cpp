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

#include "Value.h"
#include <time.h>
#include "math.h"

namespace org {
namespace hummingdroid {
namespace flightapp {

Value::Value() :
    value(0)
{
}

Value::Value(const Value &that) :
    value(that.value),
    timestamp(that.timestamp)
{
}

void Value::set(const Value & value) {
    this->value = value.value;
    this->timestamp = value.timestamp;
}

void Value::set(float value, const Timestamp & timestamp) {
    this->value = value;
    this->timestamp = timestamp;
}

void Value::add(const Value & a, const Value & b) {
    value = a.value + b.value;
    timestamp = a.timestamp;
}

void Value::limit(float min, float max) {
    if (value < min) {
        value = min;
    } else if (value > max) {
        value = max;
    }
}

void Value::amplify(const Value & value, float gain) {
    this->value = value.value * gain;
    this->timestamp = value.timestamp;
}

void Value::amplify(float gain) {
    this->value *= gain;
}

void Value::reset() {
    value = 0.;
    timestamp.t.tv_sec = 0;
    timestamp.t.tv_nsec = 0;
}

Derivator::Derivator() : prev(0.0 / 0.0)
{
}

void Derivator::derive(const Value & value)
{
    float dt = value.timestamp - timestamp;
    this->value = 2 * (value.value - prev) / dt - this->value;
    if (isnan(this->value)) {
        this->value = 0;
    }
    prev = value.value;
    this->timestamp = value.timestamp;
}

// ///////////////////////////////////////////////
// LOW PASS FILTER
// ///////////////////////////////////////////////
LowPass::LowPass(float T) : T(T)
{
}

void LowPass::setT(float T) {
    this->T = T;
}

void LowPass::lowpass(const Value & value) {
    if (!T) {
        // No filter
        this->value = value.value;
    } else {
        float K = (value.timestamp - this->timestamp) * 2.3 / T;
        if (!isnan(K)) {
            this->value = (1. - K) * this->value + K * value.value;
        } else {
            this->value = value.value;
        }
    }
    this->timestamp = value.timestamp;
}

// ///////////////////////////////////////////////
// HIGH PASS FILTER
// ///////////////////////////////////////////////

void HighPass::setT(float T) {
    lowpass.setT(T);
}

void HighPass::highpass(const Value & value) {
    lowpass.lowpass(value);
    this->value = value.value - lowpass.value;
    this->timestamp = value.timestamp;
}

// ///////////////////////////////////////////////
// PID
// ///////////////////////////////////////////////

void PID::setParams(const hummingdroid::PID & params) {
    synchronized
    this->params = params;
    low_pass.setT(params.td());
}

void PID::pid(Value value) {
    synchronized
    if (!params.IsInitialized()) {
        return;
    }
    // P
    propo.amplify(value, params.kp());
    // I
    integ.integrate(value);
    float max = 1. / params.ki();
    integ.limit(-max, max);
    integ_factor.amplify(integ, params.ki());
    // D
    low_pass.lowpass(value);
    deriv.derive(low_pass);
    deriv_factor.amplify(deriv, params.kd());
    // Sum
    this->value = propo.value + integ_factor.value + deriv_factor.value + params.ko();
    this->timestamp = value.timestamp;
}

void PID::reset() {
    synchronized
    integ.reset();
}

Integrator::Integrator() : prev(0.0 / 0.0)
{
}

void Integrator::integrate(const Value &value)
{
    float dt = value.timestamp - timestamp;
    if (!isnan(dt) && !isnan(prev)) {
        this->value += (value.value + prev) * dt / 2.;
    }
    this->timestamp = value.timestamp;
    prev = value.value;
}

void Integrator::reset()
{
    value = 0.0;
    prev = 0.0 / 0.0;
}

}
}
}
