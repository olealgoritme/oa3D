#ifndef _Editor_H
#define _Editor_H

#define ZEP_SINGLE_HEADER_BUILD
#define ZEP_FEATURE_THREADS
#define ZEP_FEATURE_CPP_FILE_SYSTEM
#include "zep/zep.h"
#include "IFileSaveCallback.h"
#include <vector>

struct Editor : Zep::IZepComponent
{
    Zep::ZepEditor_ImGui editor;

    IFileSaveCallback *fsCallback = NULL;

    // Overridden constructor
    Editor() : editor(Zep::ZepPath())
    {
        editor.RegisterCallback(this);
    };

    void addFiles(std::vector<const char *> files)
    {

        for (unsigned int i = 0; i < files.size(); i++)
            addTab(files[i]);
    }

    void setShaderFileCallback(IFileSaveCallback *callback)
    {
        this->fsCallback = callback;
    }

    virtual Zep::ZepEditor& GetEditor() const override
    {
        return (Zep::ZepEditor&) editor;
    }

    virtual void Notify(std::shared_ptr<Zep::ZepMessage> message) override
    {
        if(message->messageId == Zep::Msg::FileSave)
        {
            if(this->fsCallback != NULL)
            {
                this->fsCallback->onFileSaved();
            }
        }
        return;
    }

    void addTab(const char *path) {
        auto buffer = GetEditor().GetFileBuffer(path);
        GetEditor().AddTabWindow()->AddWindow(buffer, nullptr, false);
    }

    void registerSyntax()
    {
        Zep::RegisterSyntaxProviders(editor);
    }

};


#endif // _Editor_H

