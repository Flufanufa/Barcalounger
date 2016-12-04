#include "transform.h"

Transform::Transform(const Vector3f& _pos, const Quaternion& _rot, float _scale)
{
	pos = _pos;
	rot = _rot;
	scale = _scale;
	initializedOldStuff = false;
	parent = 0;

	parentMatrix = Matrix4f().InitIdentity();
}

bool Transform::HasChanged()
{
	if (parent != 0 && parent->HasChanged())
		return true;

	if (pos != oldPos)
		return true;

	if (rot != oldRot)
		return true;

	if (scale != scale)
		return true;

	return false;
}

void Transform::Update()
{
	if (initializedOldStuff)
	{
		oldPos = pos;
		oldRot = rot;
		oldScale = scale;
	}
	else
	{
		oldPos = pos + Vector3f(1, 1, 1);
		oldRot = rot * 0.5f;
		oldScale = scale + 1;
		initializedOldStuff = true;
	}
}

void Transform::Rotate(const Vector3f& _axis, float _angle)
{
	Rotate(Quaternion(_axis, _angle));
}

void Transform::Rotate(const Quaternion& _rotation)
{
	rot = Quaternion((_rotation * rot).Normalized());
}

void Transform::LookAt(const Vector3f& _point, const Vector3f& _up)
{
	rot = GetLookAtRotation(_point, _up);
}

Matrix4f Transform::GetTransformation() const
{
	Matrix4f translationMatrix;
	Matrix4f scaleMatrix;

	translationMatrix.InitTranslation(Vector3f(pos.GetX(), pos.GetY(), pos.GetZ()));
	scaleMatrix.InitScale(Vector3f(scale, scale, scale));

	Matrix4f result = translationMatrix * rot.ToRotationMatrix() * scaleMatrix;

	return GetParentMatrix() * result;
}

Matrix4f Transform::GetParentMatrix() const
{
	if (parent != 0 && parent->HasChanged())
		parentMatrix = parent->GetTransformation();

	return parentMatrix;
}
