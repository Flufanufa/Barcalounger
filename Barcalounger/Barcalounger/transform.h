#ifndef TRANSFORH
#define TRANSFORH

#include "math3d.h"

class Transform
{
public:
	Transform(const Vector3f& _pos = Vector3f(0, 0, 0), const Quaternion& _rot = Quaternion(0, 0, 0, 1), float _scale = 1.0f);

	Matrix4f GetTransformation() const;
	bool HasChanged();
	void Update();
	void Rotate(const Vector3f& _axis, float _angle);
	void Rotate(const Quaternion& _rotation);
	void LookAt(const Vector3f& _point, const Vector3f& _up);

	Quaternion GetLookAtRotation(const Vector3f& _point, const Vector3f& _up)
	{
		return Quaternion(Matrix4f().InitRotationFromDirection((_point - pos).Normalized(), _up));
	}

	inline Vector3f& GetPos() { return pos; }
	inline const Vector3f& GetPos() const { return pos; }
	inline Quaternion& GetRot() { return rot; }
	inline const Quaternion& GetRot() const { return rot; }
	inline float GetScale() const { return scale; }

	inline Vector3f GetTransformedPos() const { return Vector3f(GetParentMatrix().Transform(pos)); }
	inline Quaternion GetTransformedRot() const
	{
		Quaternion parentRot = Quaternion(0, 0, 0, 1);

		if (parent)
			parentRot = parent->GetTransformedRot();

		return parentRot * rot;
	}

	inline void SetPos(const Vector3f& _pos) { pos = _pos; }
	inline void SetRot(const Quaternion& _rot) { rot = _rot; }
	inline void SetScale(float _scale) { scale = _scale; }
	inline void SetParent(Transform* _parent) { parent = _parent; }
protected:
private:
	Matrix4f GetParentMatrix() const;

	Vector3f pos;
	Quaternion rot;
	float scale;

	Transform* parent;
	mutable Matrix4f parentMatrix;

	mutable Vector3f oldPos;
	mutable Quaternion oldRot;
	mutable float oldScale;
	mutable bool initializedOldStuff;
};

#endif
