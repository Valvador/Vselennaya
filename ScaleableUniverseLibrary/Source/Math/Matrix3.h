#pragma once

#include <cassert>
#include <algorithm>
#include "../ThirdParty/Math3DLib/Vector3D.h"
#include "DMatrix.h"
#include "Fmath.h"

using namespace PGSSOlver;
using namespace MATHLIB3D;
namespace ScaleableUniverse
{
	//-------------------------------------------------------------------------------------------------
	// Class Definition
	//-----
	class Matrix3 : public DMatrix
	{
	public:
		// Constructor
		Matrix3();
		Matrix3(const DMatrix& other);
		Matrix3(const Vector3D& row1, const Vector3D& row2, const Vector3D& row3);
		Matrix3(float m00, float m01, float m02,
				float m10, float m11, float m12,
				float m20, float m21, float m22);

		// Extended Operations
		inline float   determinant();
		inline Matrix3 inverse();
		inline bool matrixFEquals(const Matrix3& other, float epsilon = -1.0f);

		// Special Static 
		static inline Matrix3 identity();		
	private:
		// Constructor Helper
		inline void initMatrix3(	float m00, float m01, float m02,
									float m10, float m11, float m12,
									float m20, float m21, float m22);

	};


	//-------------------------------------------------------------------------------------------------
	// INLINED SOURCE
	//-----
	// Constructors
	Matrix3::Matrix3():DMatrix(3,3, NULL)
	{
	}

	Matrix3::Matrix3(const DMatrix& other) : DMatrix(3, 3, NULL)
	{
		assert(other.GetNumCols() == 3);
		assert(other.GetNumRows() == 3);
		Init(3, 3, other.getData());
	}

	Matrix3::Matrix3(const Vector3D& row1, const Vector3D& row2, const Vector3D& row3) : DMatrix(3, 3, NULL)
	{
		initMatrix3(row1.x, row1.y, row1.z,
					row2.x, row2.y, row2.z,
					row3.x, row3.y, row3.z);
	}

	Matrix3::Matrix3(	float m00, float m01, float m02,
						float m10, float m11, float m12,
						float m20, float m21, float m22) : DMatrix(3, 3, NULL)
	{
		initMatrix3(m00, m01, m02,
					m10, m11, m12,
					m20, m21, m22);
	}

	// Constructor Helper
	inline void Matrix3::initMatrix3(	float m00, float m01, float m02,
										float m10, float m11, float m12,
										float m20, float m21, float m22)
	{
		Set(0, 0) = m00;	Set(0, 1) = m01;	Set(0, 2) = m02;
		Set(1, 0) = m10;	Set(1, 1) = m11;	Set(1, 2) = m12;
		Set(2, 0) = m20;	Set(2, 1) = m21;	Set(2, 2) = m22;
	}

	// Extended Operations
	inline float Matrix3::determinant()
	{
		//https://www.mathworks.com/help/aeroblks/determinantof3x3matrix.html
		//		[m00, m01, m02]
		//	A = [m10, m11, m12]
		//		[m20, m21, m22]
		// det(A) = m00 * (m11*m22 - m12*m21) - m01 * (m10*m22 - m12*m20) + m02 * (m10*m21 - m11*m20)
		return	Get(0, 0) * ((Get(1, 1)*Get(2, 2)) - Get(1, 2)*Get(2, 1)) -
				Get(0, 1) * ((Get(1, 0)*Get(2, 2)) - Get(1, 2)*Get(2, 0)) +
				Get(0, 2) * ((Get(1, 0)*Get(2, 1)) - Get(1, 1)*Get(2, 0));
	}

	inline Matrix3 Matrix3::inverse()
	{
		//http://mathworld.wolfram.com/MatrixInverse.html
		float det = determinant();
		assert(det);
		Matrix3 result = Matrix3();

		//TODO Optimize
		result.Set(0, 0) = (Get(1, 1)*Get(2, 2) - Get(2, 1)*Get(1, 2));
		result.Set(0, 1) = -(Get(0, 1)*Get(2, 2) - Get(0, 2)*Get(2, 1));
		result.Set(0, 2) = (Get(0, 1)*Get(1, 2) - Get(0, 2)*Get(1, 1));
		result.Set(1, 0) = -(Get(1, 0)*Get(2, 2) - Get(1, 2)*Get(2, 0));
		result.Set(1, 1) = (Get(0, 0)*Get(2, 2) - Get(0, 2)*Get(2, 0));
		result.Set(1, 2) = -(Get(0, 0)*Get(1, 2) - Get(1, 0)*Get(0, 2));
		result.Set(2, 0) = (Get(1, 0)*Get(2, 1) - Get(2, 0)*Get(1, 1));
		result.Set(2, 1) = -(Get(0, 0)*Get(2, 1) - Get(2, 0)*Get(0, 1));
		result.Set(2, 2) = (Get(0, 0)*Get(1, 1) - Get(1, 0)*Get(0, 1));

		return result * (1 / det);
	}

	inline bool Matrix3::matrixFEquals(const Matrix3& other, float epsilon)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (!fEquals(Get(i, j), other.Get(i, j), epsilon))
				{
					return false;
				}
			}
		}
		return true;
	}

	// Special Static 
	inline Matrix3 Matrix3::identity()
	{
		Matrix3 result;
		result.Set(0, 0) = 1;
		result.Set(1, 1) = 1;
		result.Set(2, 2) = 1;
		return result;
	}
}




