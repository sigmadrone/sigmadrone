/*
 * Copyright (C) 2013 Svetoslav Vassilev
 */

#ifndef __FIRFILT_H__
#define __FIRFILT_H__

#include "matrix.h"

template <typename T, const size_t N, const size_t DIM>
class FirFilter
{
public:
	FirFilter()
	{
		Reset();
		memset(m_Coeff,0,sizeof(m_Coeff));
		m_Coeff[0] = 1.0;
	}
	FirFilter(T coeff[N])
	{
		Reset();
		memcpy(m_Coeff,coeff,sizeof(m_Coeff));
	}
	__inline ~FirFilter() {}
	__inline void Reset()
	{
		memset(m_State,0,sizeof(m_State));
		memset(m_Output,0,sizeof(m_Output));
		m_Ptr = 0;
	}
	void InitCoeff(const T coeff[N])
	{
		memcpy(m_Coeff,coeff,sizeof(m_Coeff));
		Reset();
	}
	const T* DoFilter(const T in[DIM])
	{
		for (size_t i = 0; i < DIM; i++)
		{
			m_Output[i] = 0.0;
			m_State[i][m_Ptr] = in[i];
			for (size_t j = 0; j < N; j++)
			{
				m_Output[i] += m_State[i][m_Ptr] * m_Coeff[j];
				RetreatPtr();
			}
		}
		AdvancePtr();  /*prepare for the next input sample*/
		return m_Output;
	}
	const T* /*[N]*/ GetCoeff() const { return m_Coeff; }
	const T* /*[DIM]*/ GetOutput() const { return m_Output; }

private:
	__inline void AdvancePtr() { m_Ptr = (m_Ptr + 1) % N; }
	__inline void RetreatPtr() { m_Ptr = (0 == m_Ptr) ? N-1 : m_Ptr-1; }

private:
	T m_State[DIM][N];
	T m_Coeff[N];
	T m_Output[DIM];
	size_t m_Ptr;
};

typedef FirFilter<double,11,3> FirFilter3d;

/*
 * class LpPreFilter3d
 *
 * Implements 10th order low-pass filter, with cutoff frequency
 * Fc = 0.1 * Fs / 2. The filter is designed using the windows method
 * and the frequency response does not have particularly steep slope -
 * 20 dB attenuation is achieved at 0.3*Fs/2; 40 dB @ 0.4*Fs/2
 */

class LpPreFilter3d
{
public:
	LpPreFilter3d(): m_FirFilter()
	{
		static double s_Coeff[] = {
			0.0143f, 0.0303f, 0.0723f, 0.1245f, 0.1670f, 0.1833f,
			0.1670f, 0.1245f, 0.0723f, 0.0303f, 0.0143f
		};
		m_FirFilter.InitCoeff(s_Coeff);
	}

	__inline void Reset() { m_FirFilter.Reset(); }
	const double* DoFilter(double in[3])
	{
		return m_FirFilter.DoFilter(in);
	}
	const double* DoFilter(double a, double b, double c)
	{
		double in[3] = {a,b,c};
		return m_FirFilter.DoFilter(in);
	}
	Vector3d DoFilter(const Vector3d& in)
	{
		Vector3d out;
		double inArr[3] = {in.at(0,0),in.at(1,0),in.at(2,0)};
		DoFilter(inArr);
		out.at(0,0) = m_FirFilter.GetOutput()[0];
		out.at(1,0) = m_FirFilter.GetOutput()[1];
		out.at(2,0) = m_FirFilter.GetOutput()[2];
		return out;
	}
	const double* GetCoeff() { return m_FirFilter.GetCoeff(); }
	const double* GetOutput() { return m_FirFilter.GetOutput(); }

	static const size_t s_Order = 11;
	static const size_t s_Dim = 3;

private:
	FirFilter<double, s_Order, s_Dim> m_FirFilter;
};

#endif // __FIRFILT_H__
