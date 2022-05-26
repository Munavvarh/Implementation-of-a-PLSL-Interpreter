#include <iostream>
#include <string>
#include <vector> 
#include <map>
using namespace std; 



#include "val.h"

	Value Value::operator<(const Value& op) const {
       
		if(IsReal() && op.IsInt())
		{
			return Value( (bool) (GetReal() < ((float) op.GetInt())));
		}
		else if(IsInt() && op.IsReal())
		{
			return Value( (bool) (((float) GetInt())<op.GetReal()));
		}
		else if( GetType() == op.GetType() )
        {
        	if( IsString()) 
				return Value( (bool) (Stemp < op.GetString()));
        	if( IsReal() )
				 return Value( (bool) (Rtemp < op.GetReal() ));
			if( IsInt() ) 
				return Value( (bool)(Itemp < op.GetInt() ));
		}
		return Value();
		
    }

	Value Value::operator>(const Value& op) const {
       
		if(IsReal() && op.IsInt())
		{
			return Value( (bool) (GetReal() > ((float) op.GetInt())));
		}
		else if(IsInt() && op.IsReal())
		{
			return Value( (bool) (((float) GetInt())>op.GetReal()));
		}
		else if( GetType() == op.GetType() )
        {
        	if( IsString()) 
				return Value( (bool) (Stemp > op.GetString()));
        	if( IsReal() )
				 return Value( (bool) (Rtemp > op.GetReal() ));
			if( IsInt() ) 
				return Value( (bool)(Itemp > op.GetInt() ));
		}
		return Value();
		
    }

	
	Value Value::operator-(const Value& op) const {
       
		if(IsReal() && op.IsInt())
		{
			return Value( (bool) (GetReal() - ((float) op.GetInt())));
		}
		else if(IsInt() && op.IsReal())
		{
			return Value( (bool) (((float) GetInt())-op.GetReal()));
		}
		else if( GetType() == op.GetType() )
        {
        	if( IsReal() )
				 return Value( (bool) (Rtemp - op.GetReal() ));
			if( IsInt() ) 
				return Value( (bool)(Itemp - op.GetInt() ));
		}
		return Value();
		
    }

	
 	Value Value::operator/(const Value& op) const {
       
		if(IsReal() && op.IsInt())
		{
			return Value( (bool) (GetReal() / ((float) op.GetInt())));
		}
		else if(IsInt() && op.IsReal())
		{
			return Value( (bool) (((float) GetInt())/op.GetReal()));
		}
		else if( GetType() == op.GetType() )
        {
        	if( IsReal() )
				 return Value( (bool) (Rtemp / op.GetReal() ));
			if( IsInt() ) 
				return Value( (bool)(Itemp / op.GetInt() ));
		}
		return Value();
		
    }
    


 	Value Value::operator==(const Value& op) const {
       
		if(IsReal() && op.IsInt())
		{
			return Value( (bool) (GetReal() == ((float) op.GetInt())));
		}
		else if(IsInt() && op.IsReal())
		{
			return Value( (bool) (((float) GetInt())== op.GetReal()));
		}
		else if( GetType() == op.GetType() )
        {
        	if( IsString()) 
				return Value( (bool) (Stemp == op.GetString()));
        	if( IsReal() )
				 return Value( (bool) (Rtemp == op.GetReal() ));
			if( IsInt() ) 
				return Value( (bool)(Itemp == op.GetInt() ));
		}
		return Value();
		
    }
    
    Value Value::operator*(const Value& op) const {
      
		if(IsReal() && op.IsInt())
		{
			return Value(GetReal() * (float) op.GetInt());
		}
		else if(IsInt() && op.IsReal())
		{
			return Value( (float) GetInt() * op.GetReal());
		}
		else if( GetType() == op.GetType() )
        {
        	
        	if( IsReal() ) 
				return Value( Rtemp * op.GetReal() );
			if( IsInt() ) 
				return Value( Itemp * op.GetInt() );
		}
		
		return Value();
		
    }
    

     Value Value::operator+(const Value& op) const {
       
		if(IsReal() && op.IsInt())
		{
			return Value(GetReal() + (float) op.GetInt());
		}
		else if(IsInt() && op.IsReal())
		{
			return Value( (float) GetInt() + op.GetReal());
		}
		else if( GetType() == op.GetType() )
        {
        
        	if( IsString()) 
				return Value( Stemp + op.GetString());
        	if( IsReal() )
				 return Value( Rtemp + op.GetReal() );
			if( IsInt() ) 
				return Value( Itemp + op.GetInt() );
		}
		
		return Value();
		
    }

