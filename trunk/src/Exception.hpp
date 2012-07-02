//
// Exception.hpp for  in /home/jochau_g//dev/opengl/glEngine
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 15 19:06:53 2012 gael jochaud-du-plessix
// Last update Tue Jun 26 15:41:07 2012 gael jochaud-du-plessix
//

#ifndef _GLE_EXCEPTION_HPP_
# define _GLE_EXCEPTION_HPP_

# include <exception>
# include <string>

namespace gle {
  
  namespace Exception {

    //! Describe an exception
    class Exception : public std::exception {
    public:
      //! Construct an Exception with the specified message
      Exception(std::string const message="") throw() :
	_message(message + ": Error") {}
      virtual ~Exception() throw() {}
      //! Return the message of the Exception
      virtual const char* what() const throw()
      {
	return (_message.c_str());
      }
    protected:
      //! Exeception description
      std::string _message;
    };

    //! An exception caused by an Invalid Value
    class InvalidValue : public Exception {
    public:
      //! Construct an InvalidValue exception with the specified message
      InvalidValue(std::string const message="") throw() :
	Exception(message + ": Invalid Value") {}
      virtual ~InvalidValue() throw() {}
    };

    //! An exception caused by an value out of a range
    class OutOfRange : public Exception {
    public:
      //! Construct an OutOfRange exception with the specified message
      OutOfRange(std::string const message="") throw() :
	Exception(message + ": Out of range") {}
      virtual ~OutOfRange() throw() {}
    };

    //! An exception caused by a parsing error
    class ParsingError : public Exception {
    public:
      //! Construct a ParsingError exception with the specified message
      ParsingError(std::string const message="",
		   int line = 0, std::string const & filename="") throw() :
	Exception(message + ": Parsing error"), _line(line), _filename(filename) {}
      virtual ~ParsingError() throw() {}

      //! Return the line of the error (if specified)
      int getLine() { return (_line); }

      //! Return the filename where the error occured (if specified)
      std::string const & getFilename() { return (_filename); }

    private:
      int _line;
      std::string _filename;
    };

    //! An exception caused by an OpenGL Error
    class OpenGLError : public Exception {
    public:
    //! Construct an OpenGLError exception with the specified message
      OpenGLError(std::string const message="") throw() :
	Exception(message + ": OpenGL Error") {}
      virtual ~OpenGLError() throw() {}
    };

    //! An OpenGL Error caused by a memory corruption or saturation
    class OutOfMemory : public OpenGLError {
    public:
      //! Construct an OutOfMemory exception with the specified message
      OutOfMemory(std::string const message="") throw() :
	OpenGLError(message + ": Out of memory") {}
      virtual ~OutOfMemory() throw() {}
    };

    //! An OpenGL Error caused by an invalid operation
    class InvalidOperation : public OpenGLError {
    public:
      //! Construct an InvalidOperation exception with the specified message
      InvalidOperation(std::string const message="") throw() :
	OpenGLError(message + ": Invalid Operation") {}
      virtual ~InvalidOperation() throw() {}
    };
    
    //! An OpenGL Error caused by an invalid enum parameter
    class InvalidEnum : public OpenGLError {
    public:
      //! Construct an InvalidEnum exception with the specified message
      InvalidEnum(std::string const message="") throw() :
	OpenGLError(message + ": Invalid Enum") {}
      virtual ~InvalidEnum() throw() {}
    };

    //! An OpenGL Error caused by a compilation error (typically for a shader)
    class CompilationError : public OpenGLError {
    public:
      //! Construct a CompilationError exception with the specified message
      CompilationError(std::string const message="",
		       std::string const filename="") throw() :
	OpenGLError(message + ": Compilation Error"), _filename(filename) {}
      virtual ~CompilationError() throw() {}

      //! Return the filename where the error occured (if specified)
      std::string const & getFilename() const { return (_filename); }

      //! Set the name of the file where the error occured
      void setFilename(std::string const filename="") { _filename = filename; }

    private:
      std::string _filename;
    };

    //! An OpenGL Error caused by a linkage error (typically for a program)
    class LinkageError : public OpenGLError {
    public:
      //! Construct a LinkageError exception with the specified message
      LinkageError(std::string const message="") throw() :
	OpenGLError(message + ": Linkage Error") {}
      virtual ~LinkageError() throw() {}
    };

    void CheckOpenGLError(const std::string& message="");
  };
  
};

#endif /* _GLE_EXCEPTION_HPP_ */
