//
//  HBJson.h
//  cocos2d_libs
//
//  Created by 이명열 on 2014. 8. 21..
//
//

#ifndef __cocos2d_libs__HBJson__
#define __cocos2d_libs__HBJson__

#include "cocos2d.h"
#include "external/json/document.h"

NS_CC_BEGIN

class CC_DLL HBJson : public Ref {
public:
    static HBJson* create();
    
    static Map<std::string, HBJson*> parse(std::vector<char> *data);
    static HBJson* parseValue(rapidjson::GenericDocument<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>::ConstValueIterator iterator);
    
    /** The default constructor. */
    HBJson(void);
    
    /** The default destructor */
    
    virtual ~HBJson(void);
    
    /** Initializes the HBJson */
    virtual bool init();
    
protected:
    
public:
    /** check */
    bool isBool() { if ( _t == rapidjson::kFalseType || _t == rapidjson::kTrueType ) return true; return false; }
    bool isNumber() { if ( _t == rapidjson::kNumberType ) return true; return false; }
    bool isString() { if ( _t == rapidjson::kStringType ) return true; return false;   }
    bool isObject() { if ( _t == rapidjson::kObjectType ) return true; return false;   }
    bool isArray() { if ( _t == rapidjson::kArrayType ) return true; return false;   }
    
    /** member add */
    void addData(bool data)                         {        _t = data == true ? rapidjson::kTrueType : rapidjson::kFalseType;    }
    void addData(double data)                       {        _n.d = data;           _t = rapidjson::kNumberType;    }
    void addData(int32_t data)                      {        _n.i64 = data;         _t = rapidjson::kNumberType;    }
    void addData(uint32_t data)                     {        _n.u64 = data;         _t = rapidjson::kNumberType;    }
    void addData(int64_t data)                      {        _n.i64 = data;         _t = rapidjson::kNumberType;    }
    void addData(uint64_t data)                     {        _n.u64 = data;         _t = rapidjson::kNumberType;    }
    void addData(const char *data)                  {        _s = data;             _t = rapidjson::kStringType;    }
    void addData(const char *key, HBJson *data)     {        _o.insert(key, data);  _t = rapidjson::kObjectType;    }
    void addData(HBJson *data)                      {        _a.push_back(data);    _t =rapidjson:: kArrayType;     }
    
    /** get bool */
    bool getBool() { return _t == rapidjson::kTrueType;}
    
    /** get number */
    int32_t getInt() { return (int32_t)_n.i64; }
    uint32_t getUInt() { return (uint32_t)_n.u64; }
    int64_t getInt64() { return (int64_t)_n.i64; }
    uint64_t getUInt64() { return (uint64_t)_n.u64; }
    
    float getFloat() { return (float)_n.d; }
    double getDouble() {return _n.d; }
    
    /** get string */
    const char* getString() { return _s.c_str(); }
    
    /** get object, array */
    HBJson* getObject(const char *key) { return _o.at(key); }
    HBJson* getArray(int index) { return _a[index]; }
    
    
private:
    union number {
    #if RAPIDJSON_ENDIAN == RAPIDJSON_LITTLEENDIAN
		struct I {
			int i;
			char padding[4];
		}i;
		struct U {
			unsigned u;
			char padding2[4];
		}u;
    #else
		struct I {
			char padding[4];
			int i;
		}i;
		struct U {
			char padding2[4];
			unsigned u;
		}u;
    #endif
		int64_t i64;
		uint64_t u64;
		double d;
	};
    
    
protected:
    /** String */
    std::string _s;
    
    /** Number */
    number _n;
    
    /** Object */
    Map<std::string, HBJson*> _o;
    
    /** Array */
    std::vector<HBJson*> _a;
    
    /** List */
    rapidjson::Type _t;
};

typedef Map<std::string, HBJson*> HBDocument;

NS_CC_END

#endif /* defined(__cocos2d_libs__HBJson__) */
