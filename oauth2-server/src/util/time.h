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
  * Utils for measuring and manipulating time.
  */

#pragma once
#include <atomic>
#include <thread>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>
#include <sys/timeb.h>

namespace granada{
  namespace util{

    /**
     * Utils for measuring and manipulating time.
     */
    namespace time{

      /**
       * Parse a string containing time into time_t
       * @param  time_str Stringified time
       * @return          Parsed time.
       */
      static std::time_t parse(const std::string& time_str){
        try{
          return std::stol(time_str);
        }catch(const std::logic_error e){
          return 0;
        }
      };


      /**
       * Stringify time_t.
       * @param  _time Time to stringify.
       * @return       Stringified time.
       */
      static std::string stringify(const std::time_t& _time) {
        std::stringstream ss;
        ss << _time;
        return ss.str();
      };


      /**
       * Returns true if the difference in seconds between now and the given time is higher than
       * the given timeout seconds plus an extra seconds.
       * @param  _time         Time to compare with now
       * @param  timeout       Timeout, minimum difference in seconds now and given time should have to not
       *                       being considered timedout. If -1 is considered infinite+.
       * @param  extra_seconds Extra seconds to sum to timeout.
       * @return               True if timedout false if not.
       */
      static bool is_timedout(const std::time_t& _time, const long int& timeout, const long int& extra_seconds){
        if (timeout > -1){
          std::time_t now = std::time(nullptr);
          long int seconds = (long int)std::difftime(now,_time);
          if (seconds > timeout + extra_seconds){
            return true;
          }
        }
        return false;
      };


      /**
       * Returns true if the difference in seconds between now and the given time is higher than
       * the given timeout.
       * @param  _time         Time to compare with now
       * @param  timeout       Timeout, minimum difference in seconds now and given time should have to not
       *                       being considered timedout. If -1 is considered infinite+.
       * @return               True if timedout false if not.
       */
      static inline bool is_timedout(const std::time_t& _time, const long int& timeout){
        return granada::util::time::is_timedout(_time,timeout,0);
      };


      /**
       * Returns the current time in milliseconds.
       * @return  Current time in milliseconds.
       */
      static int get_milliseconds(){
        timeb tb;
        ftime(&tb);
        return tb.millitm + (tb.time & 0xfffff) * 1000;
      };


      /**
       * Returns the span in milliseconds between the current time and
       * a given time in milliseconds.
       * @param   Time in milliseconds.
       * @return  Span in milliseconds between current time and the given time.
       */
      static int get_milliseconds_span(int start){
        int span = get_milliseconds() - start;
        if(span < 0){
          span += 0x100000 * 1000;
        }
        return span;
      };

      /**
       * Stops thread n given seconds.
       * 
       * @param seconds Seconds to stop thread.
       */
      static inline void sleep_seconds(int seconds){
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
      };


      /**
       * Stops thread n given milliseconds.
       * 
       * @param milliseconds Milliseconds to stop thread.
       */
      static inline void sleep_milliseconds(int milliseconds){
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
      };


      /**
       * Calls a function in a separate thread each n milliseconds or seconds.
       */
      class timer{
        public:
          timer(){};

          /**
           * Constructor
           * 
           * @param   fn    Function to call.
           * @param   num   Number of seconds or milliseconds to wait between
           *                each function call.
           * @param   unit  Optional. "s" by default. "s" if num is in seconds
           *                and ms if it is in milliseconds.
           */
          timer(std::function<void(void)> fn,int num,const std::string unit = "s"){
            set(std::move(fn),std::move(num),std::move(unit));
          };


          /**
           * Sets new parameters for timer.
           * 
           * @param   fn    Function to call.
           * @param   num   Number of seconds or milliseconds to wait between
           *                each function call.
           * @param   unit  Optional. "s" by default. "s" if num is in seconds
           *                and ms if it is in milliseconds.
           */
          void set(std::function<void(void)> fn,int num,const std::string unit = "s"){
            if (num>-1){
              loop_ = true;
              if (unit == "s"){
                num_ = num*1000;
              }else{
                num_ = std::move(num);
              }
              pplx::create_task([this,fn]{
                recursive(fn);
              });
            }
          };


          /**
           * Stops the timer
           */
          void stop(){
            loop_ = false;
          };

        private:

          /**
           * Number of seconds or milliseconds to wait between
           * each function call.
           */
          std::atomic_int num_;

          /**
           * If true calls the function in loop, waiting
           * a number of seconds or milliseconds between
           * each call. Function "stop" can be called to
           * break the loop.
           */
          std::atomic_bool loop_;


          /**
           * Calls a function recursively
           * 
           * @param fn Function to call.
           */
          void recursive(std::function<void(void)> fn){
            granada::util::time::sleep_milliseconds(num_);
            fn();
            if (loop_){
              recursive(fn);
            }
          };


      };


    }
  }
}
