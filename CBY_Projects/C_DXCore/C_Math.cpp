#include "C_Math.h"
namespace CMA
{
	Vector2::Vector2() 
	{
		x = y = 0.0f;
	}

	Vector2::Vector2(float fx, float fy)
	{
		x = fx;
		y = fy;
	}

	Vector2::Vector2(const Vector2& v)
	{
		x = v.x;
		y = v.y;
	}

	Vector2 Vector2::operator + (Vector2 const& v)
	{
		return Vector2(x + v.x, y + v.y);
	}

	Vector2 Vector2::operator - (Vector2 const& v)
	{
		return Vector2(x - v.x, y - v.y);
	}

	Vector2 Vector2::operator * (float const& fs)
	{
		return Vector2(x * fs, y * fs);
	}

	Vector2 Vector2::operator + (float const& fs)
	{
		return Vector2(x + fs, y + fs);
	}

	Vector2 Vector2::operator / (float const& fs)
	{
		return Vector2(x / fs, y / fs);
	}

	bool Vector2::operator == (Vector2 const& v)
	{
		if (fabs(x - v.x) <= C_EPSILON)
		{
			if (fabs(y - v.y) <= C_EPSILON)
			{
				return true;
			}
		}
		return false;
	}

	Vector3::Vector3()
	{
		x = y =z=0.0f;
	}

	Vector3::Vector3(float fx, float fy, float fz)
	{
		x = fx;
		y = fy;
		z = fz;
	}

	Vector3::Vector3(const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	float Vector3::Length()
	{
		return sqrt(x*x + y * y + z * z);
	}

	Vector3 Vector3::Nomal()
	{
		float size = 1.0f / Length();
		return Vector3(x * size, y * size, z * size);
	}

	Vector3 Vector3::operator + (Vector3 const& v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 Vector3::operator - (Vector3 const& v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 Vector3::operator * (float const& fs)
	{
		return Vector3(x * fs, y * fs, z * fs);
	}

	Vector3 Vector3::operator + (float const& fs)
	{
		return Vector3(x + fs, y + fs, z + fs);
	}

	Vector3 Vector3::operator / (float const& fs)
	{
		return Vector3(x / fs, y / fs, z / fs);
	}

	float  Vector3::operator | (Vector3 const &v)
	{
		return x * v.x, + y * v.y + z * v.z;
	}
	Vector3  Vector3::operator ^ (Vector3 const &v)
	{
		return Vector3(y*v.z - z * v.y,
						z*v.x - x * v.z,
						x*v.y - y * v.x);
	}

	bool Vector3::operator == (Vector3 const& v)
	{
		if (fabs(x - v.x) <= C_EPSILON)
		{
			if (fabs(y - v.y) <= C_EPSILON)
			{
				if (fabs(z - v.z) <= C_EPSILON)
				{
					return true;
				}
			}
		}
		return false;
	}

	Vector4::Vector4()
	{
		x = y = z = w = 0.0f;
	}

	Vector4::Vector4(float fx, float fy,float fz, float fw)
	{
		x = fx;
		y = fy;
		z = fz;
		w = fw;
	}

	Vector4::Vector4(const Vector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	Vector4 Vector4::operator + (Vector4 const& v)
	{
		return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vector4 Vector4::operator - (Vector4 const& v)
	{
		return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	Vector4 Vector4::operator * (float const& fs)
	{
		return Vector4(x * fs, y * fs, z * fs, w * fs);
	}

	Vector4 Vector4::operator + (float const& fs)
	{
		return Vector4(x + fs, y + fs, z + fs, w + fs);
	}

	Vector4 Vector4::operator / (float const& fs)
	{
		return Vector4(x / fs, y / fs, z / fs, w / fs);
	}

	bool Vector4::operator == (Vector4 const& v)
	{
		if (fabs(x - v.x) <= C_EPSILON)
		{
			if (fabs(y - v.y) <= C_EPSILON)
			{
				if (fabs(z - v.z) <= C_EPSILON)
				{
					if (fabs(w - v.w) <= C_EPSILON)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	Matrix::Matrix()
	{
		Identity();
	}

	Matrix::Matrix(const Matrix& mat)
	{
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				m[row][col] = mat.m[row][col];
			}
		}
	}

	void Matrix::Identity()
	{
		_11 = _12 = _13 = _14 = 0.0f;
		_21 = _22 = _23 = _24 = 0.0f;
		_31 = _32 = _33 = _34 = 0.0f;
		_41 = _42 = _43 = _44 = 0.0f;
		_11 = _22 = _33 = _44 = 1.0f;
	}

	void Matrix::Translation(float x, float y, float z)
	{
		Identity();
		_41 = x;
		_42 = y;
		_43 = z;
	}

	void Matrix::Translation(Vector3 v)
	{
		Identity();
		_41 = v.x;
		_42 = v.y;
		_43 = v.z;
	}

	void Matrix::Scale(float x, float y, float z)
	{
		Identity();
		_11 = x;
		_22 = y;
		_33 = z;
	}

	void Matrix::Scale(Vector3 v)
	{
		Identity();
		_11 = v.x;
		_22 = v.y;
		_33 = v.z;
	}

	void Matrix::RotateX(float rad)
	{
		Identity();
		_22 = cos(rad), _23 = sin(rad);
		_32 = -sin(rad), _33 = cos(rad);
	}

	void Matrix::RotateY(float rad)
	{
		Identity();
		_11 = cos(rad), _13 = sin(rad);
		_31 = -sin(rad), _33 = cos(rad);
	}

	void Matrix::RotateZ(float rad)
	{
		Identity();
		_11 = cos(rad), _12 = sin(rad);
		_21 = -sin(rad), _22 = cos(rad);
	}

	void Matrix::Transpose()
	{
		Matrix mat = *this;
		Matrix self;
		self.Identity();
		self._11 = mat._11; self._12 = mat._21; self._13 = mat._31; self._14 = mat._41;
		self._21 = mat._12; self._22 = mat._22; self._23 = mat._32; self._24 = mat._42;
		self._31 = mat._13; self._32 = mat._23; self._33 = mat._33; self._34 = mat._43;
		self._41 = mat._14; self._42 = mat._24; self._43 = mat._34; self._44 = mat._44;
		*this = self;
	}

	Matrix Matrix::operator *(Matrix const &mat)
	{
		Matrix re;
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				re.m[row][col] = m[row][0] * mat.m[0][col] + m[row][1] * mat.m[1][col] + m[row][2] * mat.m[2][col] + m[row][3] * mat.m[3][col];
			}
		}
		return re;
	}

	Matrix Matrix::CreateViewLookAt(Vector3& pos, Vector3& At, Vector3& up)
	{
		Matrix mat;
		Vector3 Look = (At - pos).Nomal();
		Vector3 Right = (up ^ Look).Nomal();
		Vector3 Up = (Look^Right).Nomal();

		mat._11 = Right.x; mat._12 = Up.x; mat._13 = Look.x;
		mat._21 = Right.y; mat._22 = Up.y; mat._23 = Look.y;
		mat._31 = Right.z; mat._32 = Up.z; mat._33 = Look.z;
		mat._41 = -(pos | Right);
		mat._42 = -(pos | Up);
		mat._43 = -(pos | Look);

		return mat;
	}

	Matrix Matrix::CreatePerspectiveFovLH(float Near,float Far,float FovY,float Aspect)
	{
		Matrix mat;
		ZeroMemory(&mat, sizeof(Matrix));
		float h, w, q;
		h = 1.0f / tan(FovY*0.5f);
		w = h / Aspect;
		q = Far / (Far - Near);
		mat._11 = w;
		mat._22 = h;
		mat._33 = q;
		mat._43 = -q * Near;
		mat._34 = 1;
		return mat;
	}

}


C_Math::C_Math()
{
}


C_Math::~C_Math()
{
}
