#pragma once
class Object
{
public:
	Object(double dValue);
	virtual ~Object();
	virtual void LogTransaction() = 0;
	virtual void GetValue() = 0;

private:
	double m_dValue;
};

