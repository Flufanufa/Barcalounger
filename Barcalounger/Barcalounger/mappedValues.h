#ifndef MAPPEDVALUES_H_INCLUDED
#define MAPPEDVALUES_H_INCLUDED

#include <map>
#include <string>

class MappedValues
{
public:
	inline void AddVector3f(const std::string& _name, Vector3f _value) { vector3fMap.insert(std::pair<std::string, Vector3f>(_name, _value)); }
	inline void AddFloat(const std::string& _name, float _value) { floatMap.insert(std::pair<std::string, float>(_name, _value)); }

	inline Vector3f GetVector3f(const std::string& _name) const
	{
		std::map<std::string, Vector3f>::const_iterator it = vector3fMap.find(_name);
		if (it != vector3fMap.end())
			return it->second;

		return Vector3f(0, 0, 0);
	}

	inline float GetFloat(const std::string& _name) const
	{
		std::map<std::string, float>::const_iterator it = floatMap.find(_name);
		if (it != floatMap.end())
			return it->second;

		return 0;
	}
protected:
private:
	std::map<std::string, Vector3f> vector3fMap;
	std::map<std::string, float> floatMap;
};

#endif // MAPPEDVALUES_H_INCLUDED