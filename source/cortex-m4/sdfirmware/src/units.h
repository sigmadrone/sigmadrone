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

#ifndef UNITS_H_
#define UNITS_H_

#include <stdint.h>
#include <limits>

#include <math.h>
template <typename UnitType>
struct ScaledUnit {

	static inline UnitType pico(UnitType u) { return micro(u) / 1000; }
	static inline UnitType micro(UnitType u) { return milli(u) / 1000; }
	static inline UnitType milli(UnitType u) { return u / 1000; }
	static inline UnitType kilo(UnitType u) { return u * 1000; }
	static inline UnitType mega(UnitType u) { return kilo(u) * 1000; }
	static inline UnitType giga(UnitType u) { return mega(u) * 1000; }
	static inline UnitType tera(UnitType u) { return giga(u) * 1000; }
	static inline UnitType penta(UnitType u) { return tera(u) * 1000; }

	static ScaledUnit from_picounit(UnitType pico)   { return ScaledUnit( pico/1000/1000/1000/1000); }
	static ScaledUnit from_nanounit(UnitType nano)   { return ScaledUnit( nano/1000/1000/1000); }
	static ScaledUnit from_microunit(UnitType micro) { return ScaledUnit(micro/1000/1000); }
	static ScaledUnit from_milliunit(UnitType milli) { return ScaledUnit(milli/1000); }
	static ScaledUnit from_centiunit(UnitType cent) { return ScaledUnit(cent/100); }
	static ScaledUnit from_baseunit(UnitType unit)   { return ScaledUnit( unit); }
	static ScaledUnit from_kilounit(UnitType kilo)   { return ScaledUnit( kilo*1000); }
	static ScaledUnit from_megaunit(UnitType mega)   { return ScaledUnit( mega*1000*1000); }
	static ScaledUnit from_gigaunit(UnitType giga)   { return ScaledUnit( giga*1000*1000*1000); }
	static ScaledUnit from_teraunit(UnitType tera)   { return ScaledUnit( tera*1000*1000*1000*1000); }
	static ScaledUnit from_pentaunit(UnitType penta) { return ScaledUnit(penta*1000*1000*1000*1000*1000); }

	~ScaledUnit() {}

	inline bool operator<(const ScaledUnit& rhs) const { return unit() < rhs.unit(); }
	inline bool operator==(const ScaledUnit& rhs) const { return unit() == rhs.unit(); }
	inline bool operator>(const ScaledUnit& rhs) const { return unit() > rhs.unit(); }
	inline bool operator<=(const ScaledUnit& rhs) const { return unit() <= rhs.unit(); }
	inline bool operator>=(const ScaledUnit& rhs) const { return unit() >= rhs.unit(); }
	inline bool operator!=(const ScaledUnit& rhs) const { return unit() != rhs.unit(); }

	UnitType unit() const { return units_; }

	bool is_null() const { return 0 == unit(); }

protected:
	inline UnitType femtounit() const { return picounit() / 1000; }
	inline UnitType picounit() const { return nanounit() / 1000; }
	inline UnitType nanounit() const { return microunit() / 1000; }
	inline UnitType microunit() const { return milliunit() / 1000; }
	inline UnitType milliunit() const { return unit() / 1000; }
	inline UnitType centiunit() const { return unit() / 100; }
	inline UnitType kilounit() const { return unit() * 1000; }
	inline UnitType megaunit() const { return kilounit() * 1000; }
	inline UnitType gigaunit() const { return megaunit() * 1000; }
	inline UnitType teraunit() const { return gigaunit() * 1000; }
	inline UnitType pentaunit() const { return teraunit() * 1000; }

	inline ScaledUnit(UnitType units) : units_(units) {}

private:
	UnitType units_;
};

struct TimeSpan;

struct Frequency: public ScaledUnit<uint64_t> {
	static Frequency from_microhertz(uint64_t uhz) { return Frequency(from_baseunit(uhz).unit()); }
	static Frequency from_millihertz(uint64_t millihz) {return Frequency(from_kilounit(millihz).unit()); }
	static Frequency from_hertz(uint64_t hz) { return Frequency(from_megaunit(hz).unit()); }
	static Frequency from_kilohertz(uint64_t khz) { return Frequency(from_gigaunit(khz).unit()); }
	static Frequency from_megahertz(uint64_t mhz) { return Frequency(from_teraunit(mhz).unit()); }
	static Frequency from_gigahertz(uint64_t ghz) { return Frequency(from_pentaunit(ghz).unit()); }
	static inline Frequency from_timespan(const TimeSpan& ts);
	Frequency() : ScaledUnit(0) {}
	~Frequency() {}
	uint64_t microhertz() const { return unit(); }
	uint64_t millihertz() const { return milliunit(); }
	uint64_t hertz() const { return microunit(); }
	uint64_t kilohertz() const { return nanounit(); }
	uint64_t megahertz() const { return picounit(); }
	uint64_t gigahertz() const { return femtounit(); }

	inline Frequency operator*(uint64_t rhs) const { return Frequency(unit() * rhs); }
	inline Frequency operator/(uint64_t rhs) const { return Frequency(unit() / rhs); }
	inline Frequency operator+(const Frequency& rhs) const { return Frequency(unit() + rhs.unit()); }
	inline Frequency operator-(const Frequency& rhs) const { return Frequency(unit() - rhs.unit()); }
	inline uint64_t operator/(const Frequency& rhs) const { return unit()/rhs.unit(); }

	inline TimeSpan period() const;
private:
	Frequency(uint64_t uhz) : ScaledUnit(uhz) {}
};

struct TimeSpan : public ScaledUnit<uint64_t> {
	static TimeSpan from_seconds(uint64_t secs) { return TimeSpan(from_gigaunit(secs).unit()); }
	static TimeSpan from_seconds_float(float secs) { return from_milliseconds(secs * 1000.0f); }
	static TimeSpan from_milliseconds(uint64_t millisecs) { return TimeSpan(from_megaunit(millisecs).unit()); }
	static TimeSpan from_microseconds(uint64_t usecs) { return TimeSpan(from_kilounit(usecs).unit()); }
	static TimeSpan from_nanoseconds(uint64_t nanosecs) { return TimeSpan(from_baseunit(nanosecs).unit()); }
	static TimeSpan from_ticks(uint64_t ticks, const Frequency& freq) {
		return from_nanoseconds(from_seconds(ticks).nanoseconds()/freq.hertz());
	}
	static TimeSpan from_hours(uint64_t hours) { return TimeSpan::from_seconds(hours*3600); }

	inline TimeSpan() : ScaledUnit(0) {}
	inline ~TimeSpan() {}
	float seconds_float() const { return (float)microseconds() / (1000000.0); }
	inline uint64_t seconds() const { return nanounit(); }
	inline uint64_t milliseconds() const { return microunit(); }
	inline uint64_t microseconds() const { return milliunit(); }
	inline uint64_t nanoseconds() const { return unit(); }
	inline Frequency to_frequency() const { return Frequency::from_timespan(*this); }

	inline TimeSpan operator*(uint64_t rhs) const { return TimeSpan(unit() * rhs); }
	inline TimeSpan operator/(uint64_t rhs) const { return TimeSpan(unit() / rhs); }
	inline TimeSpan operator+(const TimeSpan& rhs) const { return TimeSpan(unit() + rhs.unit()); }
	inline TimeSpan operator-(const TimeSpan& rhs) const { return TimeSpan(unit() - rhs.unit()); }
	inline uint64_t operator/(const TimeSpan& rhs) const { return unit() / rhs.unit(); }
	inline void operator+=(const TimeSpan& rhs) { *this = operator+(rhs); }
	inline void operator-=(const TimeSpan& rhs) { *this = operator+(rhs); }
	inline void operator/=(uint64_t rhs) { *this = operator/(rhs); }
	inline void operator*=(uint64_t rhs) { *this = operator*(rhs); }

private:
	inline TimeSpan(uint64_t nanosecs) : ScaledUnit(nanosecs) {}
};

TimeSpan Frequency::period() const {
	return TimeSpan::from_nanoseconds(TimeSpan::from_seconds(mega(1)).nanoseconds()/microhertz());
}

Frequency Frequency::from_timespan(const TimeSpan& ts) {
	return from_microhertz(TimeSpan::from_seconds(1000*1000).nanoseconds() / ts.nanoseconds());
}

struct Distance: public ScaledUnit<float> {
	static constexpr float INVALID_VALUE = std::numeric_limits<float>::infinity();
	static constexpr float ONE_INCH_IN_MM = 25.4f;
	static constexpr float ONE_INCH_IN_CM = ONE_INCH_IN_MM / 10.0f;
	static constexpr float ONE_FOOT_IN_CM = 12.0f * ONE_INCH_IN_CM;
	static Distance from_micrometers(float um) { return Distance(from_microunit(um).unit()); }
	static Distance from_millimeters(float mm) { return Distance(from_milliunit(mm).unit()); }
	static Distance from_centimeters(float cm) { return Distance(from_centiunit(cm).unit()); }
	static Distance from_inches(float inch) { return Distance(from_centiunit(inch * ONE_INCH_IN_CM).unit()); }
	static Distance from_feet(float feet) { return Distance(from_centiunit(feet * ONE_FOOT_IN_CM).unit()); }
	static Distance from_meters(float m) { return Distance(from_baseunit(m).unit()); }
	static Distance from_kilometers(float km) { return Distance(from_kilounit(km).unit()); }
	Distance() : ScaledUnit(0) {}
	~Distance() {}
	float micrometers() const { return megaunit(); }
	float millimeters() const { return kilounit(); }
	float centimeters() const { return centiunit(); }
	float meters() const { return unit(); }
	float kilometers() const { return milliunit(); }
	float feet() const { return centimeters() / ONE_FOOT_IN_CM; }
	float inches() const { return centimeters() / ONE_INCH_IN_CM; }

	inline Distance operator*(float rhs) const { return Distance(unit() * rhs); }
	inline const Distance& operator*=(float rhs) { *this = Distance(unit() * rhs);  return *this; }
	inline Distance operator/(float rhs) const { return Distance(unit() / rhs); }
	inline Distance operator+(const Distance& rhs) const { return Distance(unit() + rhs.unit()); }
	inline const Distance& operator+=(const Distance& rhs) { *this = Distance(unit() + rhs.unit()); return *this; }
	inline Distance operator-(const Distance& rhs) const { return Distance(unit() - rhs.unit()); }
	inline Distance operator-=(const Distance& rhs) { *this = Distance(unit() - rhs.unit()); return *this; }
	inline float operator/(const Distance& rhs) const { return unit()/rhs.unit(); }
	inline bool is_valid() const { return unit() != INVALID_VALUE; }

	Distance(int dummy) : ScaledUnit(0) {(void)dummy;}
private:
	Distance(float meters) : ScaledUnit(meters) {}
};

struct Speed : public ScaledUnit<float> {
	static constexpr float KMPH_TO_METERS_PER_SECOND = 3.6f;
	static constexpr float METERS_PER_SECOND_TO_KMPH = 1.0f / KMPH_TO_METERS_PER_SECOND;
	static Speed from_kmph(float kmph) { return Speed(kmph * KMPH_TO_METERS_PER_SECOND); }
	static Speed from_meters_per_second(float mps) { return Speed(mps); }
	Speed(const Distance& d, const TimeSpan& ts) : ScaledUnit(d.meters() / ts.seconds_float()) {}
	inline float meters_per_second() const { return unit(); }
	inline float kmph() const { return meters_per_second() * METERS_PER_SECOND_TO_KMPH; }

	Speed operator*(float rhs) const { return Speed(unit() * rhs); }
	Distance operator*(TimeSpan ts) const { return Distance::from_meters(meters_per_second() * ts.seconds_float()); }
	Speed operator*=(float rhs) { *this = Speed(unit() * rhs); return *this; }
	Speed operator/(float rhs) const { return Speed(unit() / rhs); }
	Speed operator+(const Speed& rhs) const { return Speed(unit() + rhs.unit()); }
	Speed operator+=(const Speed& rhs) { *this = Speed(unit() + rhs.unit()); return *this; }
	Speed operator-(const Speed& rhs) const { return Speed(unit() - rhs.unit()); }
	Speed operator-() const { return Speed(-unit()); }
	Speed operator-=(const Speed& rhs) { *this = Speed(unit() - rhs.unit()); return *this; }
	float operator/(const Speed& rhs) const { return unit() / rhs.unit(); }
	Speed(int dummy=0) : ScaledUnit(0) { (void)dummy;}
private:
	Speed(float meters_per_second) : ScaledUnit(meters_per_second) {}
};

typedef Distance Altitude;

struct Voltage: public ScaledUnit<float> {
	static Voltage from_microvolts(float uV) { return Voltage(from_microunit(uV).unit()); }
	static Voltage from_millivolts(float mV) { return Voltage(from_milliunit(mV).unit()); }
	static Voltage from_volts(float V) { return Voltage(from_baseunit(V).unit()); }
	static Voltage from_kilovolts(float kV) { return Voltage(from_kilounit(kV).unit()); }
	Voltage() : ScaledUnit(0) {}
	~Voltage() {}
	float microvolts() const { return megaunit(); }
	float millivolts() const { return kilounit(); }
	float volts() const { return unit(); }
	float kilovolts() const { return milliunit(); }

	inline Voltage operator*(float rhs) const { return Voltage(unit() * rhs); }
	inline Voltage operator/(float rhs) const { return Voltage(unit() / rhs); }
	inline Voltage operator+(const Voltage& rhs) const { return Voltage(unit() + rhs.unit()); }
	inline Voltage operator-(const Voltage& rhs) const { return Voltage(unit() - rhs.unit()); }
	inline float operator/(const Voltage& rhs) const { return unit()/rhs.unit(); }

private:
	Voltage(float volts) : ScaledUnit(volts) {}
};

struct Throttle: public ScaledUnit<float> {
	static constexpr float MIN_VALUE = 0.0f;
	static constexpr float MAX_VALUE = 1.0f;
	inline Throttle(float throttle = 0.0, float min = MIN_VALUE, float max = MAX_VALUE) :
			ScaledUnit(throttle < min ? min : (throttle > max ? max : throttle)) {}
	inline float get() const {return unit(); }
	inline Throttle operator-(const Throttle& rhs) const { return Throttle(unit()-rhs.unit()); }
	inline Throttle operator+(const Throttle& rhs) const { return Throttle(unit()+rhs.unit()); }
	inline float operator/(const Throttle& rhs) const { return (float)unit()/(float)rhs.unit(); }
	inline Throttle operator/(float f) const { return Throttle(unit()/f); }
	inline Throttle operator*(float f) const { return Throttle(unit()*f); }
	inline operator float() const { return get(); }
};

struct Temperature: public ScaledUnit<float> {
	static constexpr float ZERO_CELSIUS_2_KELVIN = 273.15f;

	static Temperature from_celsius(float c) { return Temperature(c); }
	static Temperature from_kelvin(float k) { return Temperature(k - ZERO_CELSIUS_2_KELVIN); }
	float celsius() const { return unit(); }
	float kelvin() const { return unit() + ZERO_CELSIUS_2_KELVIN; }
	inline Temperature operator-(const Temperature& rhs) const { return Temperature(unit()-rhs.unit()); }
	inline Temperature operator+(const Temperature& rhs) const { return Temperature(unit()+rhs.unit()); }
	inline float operator/(const Temperature& rhs) const { return (float)unit()/(float)rhs.unit(); }
	inline Temperature operator/(float f) const { return Temperature(unit()/f); }
	inline Temperature operator*(float f) const { return Temperature(unit()*f); }
	inline Temperature() : ScaledUnit<float>(0) {}
	inline Temperature(int dummy): ScaledUnit(0) { (void)dummy; }
private:
	Temperature(float celsius) : ScaledUnit<float>(celsius) {}
};

struct Pressure:  public ScaledUnit<float> {
	static constexpr float PSI_TO_PA = 6894.76f;
	static constexpr float PA_TO_PSI = 1.0f / PSI_TO_PA;

	static Pressure from_psi(float psi) { return Pressure(psi * PSI_TO_PA); }
	static Pressure from_hpa(float hpa) { return Pressure(from_centiunit(hpa).unit()); }
	static Pressure from_mpa(float kpa) { return Pressure(from_megaunit(kpa).unit()); }
	static Pressure from_pa(float pa) { return Pressure(pa); }
	inline float hpa() const { return centiunit(); }
	inline float kpa() const { return hpa(); }
	inline float mpa() const { return megaunit(); }
	inline float pa() const { return unit(); }
	inline float psi() const { return unit() * PA_TO_PSI; }
	inline Pressure operator-(const Pressure& rhs) const { return Pressure(unit()-rhs.unit()); }
	inline Pressure operator+(const Pressure& rhs) const { return Pressure(unit()+rhs.unit()); }
	inline float operator/(const Pressure& rhs) const { return (float)unit()/(float)rhs.unit(); }
	inline Pressure operator/(float f) const { return Pressure(unit()/f); }
	inline Pressure operator*(float f) const { return Pressure(unit()*f); }
	inline void operator-=(const Pressure& rhs) { *this = Pressure(unit()-rhs.unit()); }
	inline void operator+=(const Pressure& rhs) { *this = Pressure(unit()+rhs.unit()); }
	inline void operator/=(float f) { *this = Pressure(unit()/f); }
	inline void operator*=(float f) { *this = Pressure(unit()*f); }

	inline Pressure() : ScaledUnit(0) {}
	inline Pressure(int dummy): ScaledUnit(0) { (void)dummy; }
private:
	Pressure(float pascal) : ScaledUnit<float> (pascal) {}
};

struct Angle
{
	static constexpr float PI = 3.1415926f;
	static constexpr float INVALID_VALUE = std::numeric_limits<float>::infinity();

	static Angle from_degrees(float deg) { return Angle(deg); }
	static Angle from_radians(float rad) { return Angle(rad * 180.0f / PI); }
	float degrees() const { return angle_; }
	float radians() const { return angle_ * PI / 180.0f; }
	inline Angle operator-(const Angle& rhs) const { return Angle(degrees()-rhs.degrees()); }
	inline Angle operator+(const Angle& rhs) const { return Angle(degrees()+rhs.degrees()); }
	inline float operator/(const Angle& rhs) const { return (float)degrees()/(float)rhs.degrees(); }
	inline Angle operator/(float f) const { return Angle(degrees()/f); }
	inline Angle operator*(float f) const { return Angle(degrees()*f); }
	inline void operator-=(const Angle& rhs) { *this = Angle(degrees()-rhs.degrees()); }
	inline void operator+=(const Angle& rhs) { *this = Angle(degrees()+rhs.degrees()); }
	inline void operator/=(float f) { *this = Angle(degrees()/f); }
	inline void operator*=(float f) { *this = Angle(degrees()*f); }
	inline bool operator<(const Angle& rhs) const { return angle_ < rhs.angle_; }
	inline bool operator<=(const Angle& rhs) const { return angle_ <= rhs.angle_; }
	inline bool operator>(const Angle& rhs) const { return angle_ > rhs.angle_; }
	inline bool operator>=(const Angle& rhs) const { return angle_ >= rhs.angle_; }
	inline bool operator==(const Angle& rhs) const { return angle_ == rhs.angle_; }
	Angle() : angle_(0) {}
	Angle(int dummy) : angle_(0) { (void)dummy; }
private:
	Angle(float angleInDegrees): angle_(angleInDegrees) {}
	float angle_;
};

static const Angle INVALID_ANGLE = Angle::from_radians(Angle::INVALID_VALUE);
static const Altitude INVALID_ALTITUDE = Altitude::from_meters(Altitude::INVALID_VALUE);
static const Distance ONE_METER = Distance::from_meters(1.0f);
static const TimeSpan ONE_SECOND = TimeSpan::from_seconds(1.0f);
static const Voltage ONE_VOLT = Voltage::from_volts(1.0f);
static const Frequency ONE_HERTZ = Frequency::from_hertz(1.0f);
static const Pressure PRESSURE_SEA_LEVEL = Pressure::from_pa(101325.0f);

struct Latitude
{
	static bool is_valid_coordinate(const Angle& a = INVALID_ANGLE)
	{
		return fabs(a.radians()) <= Angle::PI/2;
	}
	Latitude(const Angle& lat = INVALID_ANGLE) : latitude_(is_valid_coordinate(lat) ? lat : INVALID_ANGLE) {}
	const Angle& get() const { return latitude_; }
	float radians() const { return latitude_.radians(); }
	float degrees() const { return latitude_.degrees(); }
	bool is_valid() const { return is_valid_coordinate(latitude_); }
private:
	Angle latitude_;
};

struct Longitude
{
	static bool is_valid_coordinate(const Angle& a)
	{
		return fabs(a.radians()) <= Angle::PI;
	}
	Longitude(const Angle& lon = INVALID_ANGLE) : longitude_(is_valid_coordinate(lon) ? lon : INVALID_ANGLE) {}
	const Angle& get() const { return longitude_; }
	float radians() const { return longitude_.radians(); }
	float degrees() const { return longitude_.degrees(); }
	bool is_valid() const { return is_valid_coordinate(longitude_); }
private:
	Angle longitude_;
};


inline Altitude pressure_to_altitude(
		const Pressure& pressure,
		const Pressure& base_pressure = PRESSURE_SEA_LEVEL,
		const Temperature& t = Temperature::from_celsius(15.0f))
{
	return Altitude::from_meters((powf(base_pressure/pressure,0.1902f) - 1.0f) * t.kelvin() / 0.0065f);
}

inline Speed operator/(const Distance& distance, const TimeSpan& time) { return Speed(distance, time); }

template<typename T>
T unit_min(T a, T b)
{
	return a <= b ? a : b;
}

template<typename T>
T unit_max(T a, T b)
{
	return a >= b ? a : b;
}

#endif /* UNITS_H_ */
