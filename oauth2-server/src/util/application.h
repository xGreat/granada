/**
  * Copyright (c) <2016> Web App SDK granada <afernandez@cookinapps.io>
  *
  * This source code is licensed under the MIT license.
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in
  * all copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  *
  * Collection of functions and classes related to application properties such
  * as the application configuration properties and the application paths.
  *
  */

#pragma once

#include <memory>
#include <string>

#ifdef __APPLE__
  #include <libproc.h>
  #include <unistd.h>
#elif _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
  #include <windows.h>
#else
  #include <linux/limits.h>
#endif
#include "cpprest/details/basic_types.h"
#include "file.h"

namespace granada{
  namespace util{

    /**
     * Utils for application.
     */
    namespace application{

      /**
       * Path of the application.
       */
      static std::string selfpath;


      /**
       * Application property file.
       */
      static std::unique_ptr<granada::util::file::PropertyFile> property_file_;


      /**
       * Returns the path of the application.
       * @return Path.
       */
      const std::string& get_selfpath();


      /**
       * Returns the value of a property of the application config file.
       * @param  name Name of the property to retrieve.
       * @return      Value of the property.
       */
      const std::string GetProperty(const std::string& name);


      /**
       * Homogenize a given path in the form of string to a format like: /path/to/directory
       * Example: /my/path  =>  will stay as it is, the / at the begining is interpreted as if
       *                        we had been given a complete path.
       *          my/path   =>  /path/to/application/directory/my/path The absence of / at the
       *                        begining is interpreted as if we had been given a relative path.
       *          /my/path/ => /my/path   Last / is always removed.
       */
      static const std::string FormatDirectoryPath(const std::string& directory_path){
        std::string formatted_directory_path = directory_path;
        if (!directory_path.empty()){

          // if the repository path ends with a slash remove it.
          if (formatted_directory_path.back() == '/'){
            formatted_directory_path.pop_back();
          }
          
          // if the repository does not start with a slash
          // we supose it is a relative path
          if (formatted_directory_path[0] != '/'){

            // complete the path adding the application directory path before the given relative path.
            formatted_directory_path = granada::util::application::get_selfpath() + "/" + formatted_directory_path;
          }
        }

        return formatted_directory_path;
      };
    }
  }
}