#pragma once


#include "Extras/nlohmann/json.hpp"
#include "Source/Core/Strings/CRStringInc.h"
#include <cstddef>


namespace UtilJson
{
    /// Return true when json value is an object.
    inline bool IsObject( const nlohmann::json& JsonValue )
    {
        return JsonValue.is_object();
    }

    /// Return true when object has integer field.
    inline bool IsIntegerField( const nlohmann::json& JsonObject, const CRString& FieldName )
    {
        if ( !JsonObject.is_object() ) return false;
        if ( !JsonObject.contains( FieldName ) ) return false;

        return JsonObject[ FieldName ].is_number_integer();
    }

    /// Return true when object has array field.
    inline bool IsValidArray( const nlohmann::json& JsonObject, const CRString& FieldName, size_t MinSize = 0 )
    {
        if ( !JsonObject.is_object() ) return false;
        if ( !JsonObject.contains( FieldName ) ) return false;

        const nlohmann::json& fieldValue = JsonObject[ FieldName ];
        if ( !fieldValue.is_array() ) return false;

        return fieldValue.size() >= MinSize;
    }
}
