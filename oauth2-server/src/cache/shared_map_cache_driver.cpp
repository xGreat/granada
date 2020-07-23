/**
  * Copyright (c) <2016> granada <afernandez@cookinapps.io>
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
  * Manages the cache
  */

#include "cache/shared_map_cache_driver.h"

namespace granada{
  namespace cache{

    SharedMapIterator::SharedMapIterator(const std::string& expression, SharedMapCacheDriver* cache){
      cache_ = cache;
      set(expression);
    }


    void SharedMapIterator::set(const std::string& expression){
      expression_ = expression;
      std::deque<std::pair<std::string,std::string>> values;
      values.push_back(std::make_pair("*",".*"));
      granada::util::string::replace(expression_,values,"","");
      cache_->Keys(expression_,keys_);
      it_ = keys_.begin();
    }


    const bool SharedMapIterator::has_next(){
      return it_ != keys_.end();
    }


    const std::string SharedMapIterator::next(){
      if (it_ != keys_.end()){
        const std::string value(*it_);
        ++it_;
        return value;
      }
      return std::string();
    }


    SharedMapCacheDriver::SharedMapCacheDriver(){
      data_.reset(new std::unordered_map<std::string,std::map<std::string,std::string>>());
    }


    const bool SharedMapCacheDriver::Exists(const std::string& key){
      std::lock_guard<std::mutex> lg(mtx_);
      if (data_->find(key) != data_->end()){
        return true;
      }
      return false;
    }


    const bool SharedMapCacheDriver::Exists(const std::string& hash,const std::string& key){
      std::lock_guard<std::mutex> lg(mtx_);
      auto it = data_->find(hash);
      if (it != data_->end()){
        const std::map<std::string,std::string>& properties = it->second;
        auto it2 = properties.find(key);
        if(it2 != properties.end()){
          return true;
        }
      }
      return false;
    }


    const std::string SharedMapCacheDriver::Read(const std::string& key){
      std::lock_guard<std::mutex> lg(mtx_);
      auto it = data_->find(key);
      if (it != data_->end()){
        const std::map<std::string,std::string>& properties = it->second;
        auto it2 = properties.find("__");
        if(it2 != properties.end()){
          return it2->second;
        }
      }
      return std::string();
    }


    const std::string SharedMapCacheDriver::Read(const std::string& hash,const std::string& key){
      std::lock_guard<std::mutex> lg(mtx_);
      auto it = data_->find(hash);
      if (it != data_->end()){
        std::map<std::string,std::string> properties = it->second;
        auto it2 = properties.find(key);
        if(it2 != properties.end()){
          return it2->second;
        }
      }
      return std::string();
    }


    void SharedMapCacheDriver::Write(const std::string& key,const std::string& value){
      std::lock_guard<std::mutex> lg(mtx_);
      auto it = data_->find(key);
      if (it == data_->end()){
        std::map<std::string,std::string> properties;
        properties["__"] = value;
        (*data_)[key] = properties;
      }else{
        std::map<std::string,std::string> properties = it->second;
        properties["__"] = value;
        (*data_)[key] = properties;
      }
    }


    void SharedMapCacheDriver::Write(const std::string& hash,const std::string& key,const std::string& value){
      std::lock_guard<std::mutex> lg(mtx_);
      auto it = data_->find(hash);
      if (it == data_->end()){
        std::map<std::string,std::string> properties;
        properties[key] = value;
        (*data_)[hash] = properties;
      }else{
        std::map<std::string,std::string> properties = it->second;
        properties[key] = value;
        (*data_)[hash] = properties;
      }
    }
    

    void SharedMapCacheDriver::Destroy(const std::string& key){
      std::size_t found = key.find("*");
      if (found!=std::string::npos){
        std::vector<std::string> keys;
        Match(key,keys);
        for (auto it = keys.begin(); it != keys.end(); ++it){
          std::lock_guard<std::mutex> lg(mtx_);
          data_->erase(*it);
        }
      }else{
        std::lock_guard<std::mutex> lg(mtx_);
        data_->erase(key);
      }
    }


    void SharedMapCacheDriver::Destroy(const std::string& hash,const std::string& key){
      std::lock_guard<std::mutex> lg(mtx_);
      auto it = data_->find(hash);
      if (it != data_->end()){
        std::map<std::string,std::string> properties = it->second;
        properties.erase(key);
        (*data_)[hash] = properties;
      }
    }


    bool SharedMapCacheDriver::Rename(const std::string& old_key, const std::string& new_key){
      std::lock_guard<std::mutex> lg(mtx_);
      auto it = data_->find(old_key);
      if (it != data_->end()) {
        // insert new key and value
        std::map<std::string,std::string> properties;
        (*data_)[new_key] = properties;

        // swap
        std::swap((*data_)[new_key], it->second);

        // erase old entry
        data_->erase(it);
        return true;
      }
      return false;
    }


    void SharedMapCacheDriver::Keys(const std::string& expression, std::vector<std::string>& keys){
      keys.clear();
      std::lock_guard<std::mutex> lg(mtx_);
      for(auto it = data_->begin(); it != data_->end(); ++it) {
        const std::string& key = it->first;
        if (std::regex_match(key, std::regex(expression))){
          keys.push_back(it->first);
        }
      }
    }

  }
}
