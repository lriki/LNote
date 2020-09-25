//=============================================================================
//Åy INotifyPropertyChanged Åz
//-----------------------------------------------------------------------------
///**
//  @file       INotifyPropertyChanged.h
//  @brief      INotifyPropertyChanged
//  @author     Riki
//*/
//=============================================================================

#pragma once

#include "../../Base/Delegate.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace GUI
{

class BindingSourceValue;
//typedef Base::Delegate01< const lnChar* > PropertyChangedDelegate;
//typedef Base::Event01< const lnChar* > PropertyChangedEventHandler;

/// IReceivePropertyChanged
//class IReceivePropertyChanged
//{
//public:
//
//    virtual ~IReceivePropertyChanged() {}
//
//public:
//
//    virtual void onContextPropertyChanged( const lnChar* property_name_ ) = 0;
//};

class BindingTargetElement
{
public:

    BindingTargetElement()
        : mLockedBindingToSource( false )
    {}

    virtual ~BindingTargetElement() {}

public:

    void lockBindingToSource() { mLockedBindingToSource = true; }

    void unlockBindingToSource() { mLockedBindingToSource = false; }

    bool isLockedBindingToSource() const { return mLockedBindingToSource; }

public:

    virtual void onContextPropertyChanged( const lnChar* property_name_ ) = 0;

private:

    bool    mLockedBindingToSource;
};

/// PropertyChangedEventHandler
class PropertyChangedEventHandler
{
public:

    void operator += ( BindingTargetElement* obj_ )
    {
        mHandlerArray.push_back( obj_ );
    }

    void operator -= ( BindingTargetElement* obj_ )
    {
        Base::STLUtil::remove( mHandlerArray, obj_ );
    }

    void operator () ( const lnChar* property_name_ )
	{
        if ( !mHandlerArray.empty() )
		{
            ln_foreach( BindingTargetElement* h, mHandlerArray )
            {
                h->lockBindingToSource();
                h->onContextPropertyChanged( property_name_ );
                h->unlockBindingToSource();
            }
		}
	}

private:

    std::vector< BindingTargetElement* > mHandlerArray;
};

/// INotifyPropertyChanged
class INotifyPropertyChanged
{
public:

    virtual ~INotifyPropertyChanged() {}

public:
    
    PropertyChangedEventHandler PropertyChanged;

    //virtual void attachPropertyChangedHandler( PropertyChangedHandler method_ ) = 0;

    virtual const lnChar* getPropertyValue( const LRefTString& property_name_ ) = 0;

protected:

    virtual void onPropertyChanged( const lnChar* property_name_ ) = 0;

public:

    virtual void onPropertyApplied( const LRefTString& property_name_, const BindingSourceValue& value_ ) = 0;
};

/// BasicNotifyPropertyChanged
class BasicNotifyPropertyChanged
    : public INotifyPropertyChanged
{
public:

    //virtual void attachPropertyChangedHandler( PropertyChangedHandler method_ )
    //{
    //    mHandlerMethodArray.push_back( method_ );
    //}

protected:

    virtual void onPropertyChanged( const lnChar* property_name_ )
    {
        PropertyChanged( property_name_ );
        //if ( !mHandlerMethod.isEmpty() )
        //    mHandlerMethod.call( property_name_ );
    }

private:

    //typedef std::vector< PropertyChangedHandler > PropertyChangedHandlerArray;
    //PropertyChangedHandlerArray mHandlerMethodArray;
};

} // namespace GUI
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================