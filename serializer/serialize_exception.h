#ifndef SERIALIZE_EXCEPTION_H
#define SERIALIZE_EXCEPTION_H

class DeserializeException :public std::exception {};

class RAWDeserializationException :public DeserializeException
{
public :const char *what() const noexcept {return "error of raw data type deserialization";}
};

class RAWSeserializationException :public DeserializeException
{
public :const char *what() const noexcept {return "nullptr";}
};

class VectorDeserializationException :public DeserializeException
{
public :const char *what() const noexcept {return "error of vector deserialization";}
};

class MapDeserializationException :public DeserializeException
{
public :const char *what() const noexcept {return "error of map deserialization";}
};


class StringDeserializationException :public DeserializeException
{
public :const char *what() const noexcept {return "error of string deserialization";}
};

#endif // ERIALIZE_EXCEPTION_H
