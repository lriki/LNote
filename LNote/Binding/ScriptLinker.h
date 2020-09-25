//=============================================================================
//Åy ScriptLinker Åz
//-----------------------------------------------------------------------------
///**
//  @file       ScriptLinker.h
//  @brief      ScriptLinker
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Binding
{

//=============================================================================
// ScriptLinker
//=============================================================================
class ScriptLinker
{
public:

    ScriptLinker();

    ~ScriptLinker();

public:

    void initialize(Core::File::Manager* file_manager_);

    void addFile(const lnChar* filename_);

    void clear();

    const char* getScriptCode() const { return mScript.c_str(); }

private:

    struct Entry
    {
        int             StartLine;
        int             LineCount;
        ln_std_tstring  Filename;
    };

    typedef std::vector<Entry>  SourceEntryArray;

private:

    Core::File::Manager*    mFileManager;
    std::string             mScript;
    int                     mLastLine;
    SourceEntryArray        mSourceEntryArray;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Binding
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================