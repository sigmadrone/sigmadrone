/*
 * units.h
 *
 *  Created on: Jan 17, 2015
 *      Author: svetlio
 */

#ifndef UNITS_H_
#define UNITS_H_

#include <stdint.h>
#include <limits>

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
	static TimeSpan from_milliseconds(uint64_t millisecs) { return TimeSpan(from_megaunit(millisecs).unit()); }
	static TimeSpan from_microseconds(uint64_t usecs) { return TimeSpan(from_kilounit(usecs).unit()); }
	static TimeSpan from_nanoseconds(uint64_t nanosecs) { return TimeSpan(from_baseunit(nanosecs).unit()); }
	static TimeSpan from_ticks(uint64_t ticks, const Frequency& freq) {
		return from_nanoseconds(from_seconds(ticks).nanoseconds()/freq.hertz());
	}

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
	inline Distance operator/(float rhs) const { return Distance(unit() / rhs); }
	inline Distance operator+(const Distance& rhs) const { return Distance(unit() + rhs.unit()); }
	inline Distance operator-(const Distance& rhs) const { return Distance(unit() - rhs.unit()); }
	inline float operator/(const Distance& rhs) const { return unit()/rhs.unit(); }
	inline bool is_valid() const { return unit() != INVALID_VALUE; }

private:
	Distance(float meters) : ScaledUnit(meters) {}
};

typedef Distance Altitude;

static const Altitude INVALID_ALTITUDE = Altitude::from_meters(Altitude::INVALID_VALUE);

struct Voltage: public ScaledUnit<float> {
	static Voltage from_microvolts(float uV) { return Voltage(from_microunit(uV).unit()); }
	static Voltage from_millvolts(float mV) { return Voltage(from_milliunit(mV).unit()); }
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

#endif /* UNITS_H_ */
