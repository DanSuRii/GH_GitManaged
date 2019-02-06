#include <iostream>

class AbstractDispacher; // Forward declare AbstractDispacher

class File
{
public:
    virtual void accept( AbstractDispacher& ) = 0;
};

class ArchivedFile;
class SplitFile;
class ExtractedFile;

class AbstractDispacher{
public:
    // Declare overloads for each kind of a file to dispatch
    virtual void dispatch( ArchivedFile &file ) = 0;
    virtual void dispatch( SplitFile &file ) = 0;
    virtual void dispatch( ExtractedFile &file ) = 0;
};

class ArchivedFile : public File
{
public:
    // Resolved at runtime, it calls the dispacher's overloaded function, corresponding to ArchivedFile
    void accept(AbstractDispacher& dispatcher) override {
        dispatcher.dispatch(*this);
    }
};

class SplitFile : public File
{
public:
    // Resolved at runtime, it calls the dispacher's overloaded function, corresponding to SplitFile
      void accept(AbstractDispacher& dispatcher) override {
        dispatcher.dispatch(*this);
    }  
};

class ExtractedFile : public File
{
public:
    // Resolved at runtime, it calls the dispacher's overloaded function, corresponding to ExtractedFile
      void accept(AbstractDispacher& dispatcher) override {
        dispatcher.dispatch(*this);
    }  
};

class Dispatcher : public AbstractDispacher{
public:
    void dispatch( ArchivedFile& ) override{
        std::cout << "dispatching ArchivedFile" << std::endl;
    }

    void dispatch( SplitFile& ) override{
        std::cout << "dispatching SplitFile" << std::endl;
    }

    void dispatch( ExtractedFile& ) override{
        std::cout << "dispatching ExtractedFile" << std::endl;
    }
};

template<class T, class ... Args>
auto New(Args&& ... args) 
{
    return std::make_shared< T >();
}

int main()
{
    std::shared_ptr<File> files[]{
        New<ArchivedFile>(),
        New<SplitFile>(),
        New<ExtractedFile>(),
    };

    Dispatcher dispatcher;

    for( auto cur : files )
    {
        cur->accept(dispatcher);
    }
}