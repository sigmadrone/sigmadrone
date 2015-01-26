/*
 * units.h
 *
 *  Created on: Jan 17, 2015
 *      Author: svetlio
 */

#ifndef UNITS_H_
#define UNITS_H_

template <typename UnitType>
struct ScaledUnit {
	static ScaledUnit from_baseunit(UnitType unit) { return ScaledUnit(unit); }
	static ScaledUnit from_milliunit(UnitType milli) { return ScaledUnit(milli/1000); }
	static ScaledUnit from_microunit(UnitType micro) { return ScaledUnit(micro/1000/1000); }
	static ScaledUnit from_nanounit(UnitType nano) { return ScaledUnit(nano/1000/1000/10000); }
	static ScaledUnit from_kilounit(UnitType kilo) { return ScaledUnit(kilo*1000); }
	static ScaledUnit from_megaunit(UnitType mega) { return ScaledUnit(mega*1000*1000); }
	static ScaledUnit from_gigaunit(UnitType giga) { return ScaledUnit(giga*1000*1000*1000); }

	~ScaledUnit() {}

	inline bool operator<(const ScaledUnit& rhs) const { return unit() < rhs.unit(); }
	inline bool operator==(const ScaledUnit& rhs) const { return unit() == rhs.unit(); }
	inline bool operator>(const ScaledUnit& rhs) const { return unit() > rhs.unit(); }
	inline bool operator<=(const ScaledUnit& rhs) const { return unit() <= rhs.unit(); }
	inline bool operator>=(const ScaledUnit& rhs) const { return unit() >= rhs.unit(); }
	inline bool operator!=(const ScaledUnit& rhs) const { return unit() != rhs.unit(); }

	UnitType unit() const { return units_; }

protected:
	UnitType milliunit() const { return unit() / 1000; }
	UnitType microunit() const { return milliunit() / 1000; }
	UnitType nanounit() const { return microunit() / 1000; }
	UnitType kilounit() const { return unit() * 1000; }
	UnitType megaunit() const { return kilounit() * 1000; }
	UnitType gigaunit() const { return megaunit() * 1000; }
	ScaledUnit(UnitType units) : units_(units) {}

private:
	UnitType units_;
};

struct TimeSpan;

struct Frequency: public ScaledUnit<uint64_t> {
	static Frequency from_hertz(uint64_t hz) { return Frequency(from_baseunit(hz).unit()); }
	static Frequency from_kilohertz(uint64_t khz) { return Frequency(from_kilounit(khz).unit()); }
	static Frequency from_megahertz(uint64_t mhz) { return Frequency(from_megaunit(mhz).unit()); }
	static Frequency from_gigahertz(uint64_t ghz) { return Frequency(from_gigaunit(ghz).unit()); }
	static inline Frequency from_timespan(const TimeSpan& ts);
	Frequency() : ScaledUnit(0) {}
	~Frequency() {}
	uint64_t hertz() const { return unit(); }
	uint64_t kilohertz() const { return kilounit(); }
	uint64_t megahertz() const { return megaunit(); }
	uint64_t gigahertz() const { return gigaunit(); }

	inline Frequency operator*(uint64_t rhs) const { return Frequency(unit() * rhs); }
	inline Frequency operator/(uint64_t rhs) const { return Frequency(unit() / rhs); }
	inline Frequency operator+(const Frequency& rhs) const { return Frequency(unit() + rhs.unit()); }
	inline Frequency operator-(const Frequency& rhs) const { return Frequency(unit() - rhs.unit()); }

	inline TimeSpan period();
private:
	Frequency(uint64_t hz) : ScaledUnit(hz) {}
};

struct TimeSpan : public ScaledUnit<uint64_t> {
	static TimeSpan from_seconds(uint64_t secs) { return TimeSpan(from_gigaunit(secs).unit()); }
	static TimeSpan from_milliseconds(uint64_t millisecs) { return TimeSpan(from_megaunit(millisecs).unit()); }
	static TimeSpan from_microseconds(uint64_t usecs) { return TimeSpan(from_kilounit(usecs).unit()); }
	static TimeSpan from_nanoseconds(uint64_t nanosecs) { return TimeSpan(from_baseunit(nanosecs).unit()); }
	static TimeSpan from_ticks(uint64_t ticks, const Frequency& freq) {
		return from_nanoseconds(from_seconds(ticks).nanoseconds()/freq.hertz());
	}

	TimeSpan() : ScaledUnit(0) {}
	~TimeSpan() {}
	uint64_t seconds() const { return nanounit(); }
	uint64_t milliseconds() const { return microunit(); }
	uint64_t microseconds() const { return milliunit(); }
	uint64_t nanoseconds() const { return unit(); }
	bool is_null() const { return !unit(); }

	inline TimeSpan operator*(uint64_t rhs) const { return TimeSpan(unit() * rhs); }
	inline TimeSpan operator/(uint64_t rhs) const { return TimeSpan(unit() / rhs); }
	inline TimeSpan operator+(const TimeSpan& rhs) const { return TimeSpan(unit() + rhs.unit()); }
	inline TimeSpan operator-(const TimeSpan& rhs) const { return TimeSpan(unit() - rhs.unit()); }
	inline uint64_t operator/(const TimeSpan& rhs) const { return unit() / rhs.unit(); }

private:
	TimeSpan(uint64_t nanosecs) : ScaledUnit(nanosecs) {}
};

TimeSpan Frequency::period() {
	return TimeSpan::from_nanoseconds(TimeSpan::from_seconds(1).nanoseconds()/hertz());
}

Frequency Frequency::from_timespan(const TimeSpan& ts) {
	return from_hertz(TimeSpan::from_seconds(1).nanoseconds() / ts.nanoseconds());
}

#endif /* UNITS_H_ */
