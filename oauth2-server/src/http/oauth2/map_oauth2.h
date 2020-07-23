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
  * Classes of entities useful for OAuth 2.0 authorization,
  * use maps to store data.
  * Based on rfc6749 document: The OAuth 2.0 Authorization Framework
  * https://tools.ietf.org/html/rfc6749
  *
  */

#pragma once
#include "util/mutex.h"
#include "http/oauth2/oauth2.h"
#include "cache/shared_map_cache_driver.h"
#include "crypto/nonce_generator.h"
#include "crypto/openssl_aes_cryptograph.h"

namespace granada{

  namespace http{

    namespace oauth2{

      class MapOAuth2Client : public OAuth2Client{
        public:
          /**
           * Constructor
           * Initialize nonce generator and load properties.
           */
          MapOAuth2Client(){
            MapOAuth2Client::load_properties_call_once_.call([this](){
              this->LoadProperties();
            });
          };


          /**
           * Constructor
           * Initialize nonce generator, load properties and load client with the given id.
           */
          MapOAuth2Client(const std::string& id){
            MapOAuth2Client::load_properties_call_once_.call([this](){
              this->LoadProperties();
            });
            id_ = id;
            Load();
          };

          // override
          virtual granada::cache::CacheHandler* cache() override {
            return cache_.get();
          };

          // override
          virtual granada::crypto::Cryptograph* cryptograph() override {
            return cryptograph_.get();
          };

          // override
          virtual granada::crypto::NonceGenerator* nonce_generator() override {
            return n_generator_.get();
          };


        private:

          /**
           * Used for loading the properties only once.
           */
          static granada::util::mutex::call_once load_properties_call_once_;


          /**
           * Cache to insert, modify and delete client data.
           */
          static std::unique_ptr<granada::cache::CacheHandler> cache_;


          /**
           * Cryptograph to encrypt and decrypt client credentials.
           */
          static std::unique_ptr<granada::crypto::Cryptograph> cryptograph_;


          /**
           * Nonce string generator, for generating unique strings tokens.
           * Generate a nonce string containing random alphanumeric characters (A-Za-z0-9).
           */
          static std::unique_ptr<granada::crypto::NonceGenerator> n_generator_;
      };


      class MapOAuth2User : public OAuth2User{
        public:
          /**
           * Constructor
           * Initialize nonce generator and load properties.
           */
          MapOAuth2User(){
            MapOAuth2User::load_properties_call_once_.call([this](){
              this->LoadProperties();
            });
          };


          /**
           * Constructor
           * Initialize nonce generator, load properties and load user with the given username.
           */
          MapOAuth2User(const std::string& username){
            MapOAuth2User::load_properties_call_once_.call([this](){
              this->LoadProperties();
            });
            username_ = username;
            Load();
          };

          // override
          virtual granada::cache::CacheHandler* cache() override {
            return cache_.get();
          };

          // override
          virtual granada::crypto::Cryptograph* cryptograph() override {
            return cryptograph_.get();
          };

          // override
          virtual granada::crypto::NonceGenerator* nonce_generator() override {
            return n_generator_.get();
          };



        private:

          /**
           * Used for loading the properties only once.
           */
          static granada::util::mutex::call_once load_properties_call_once_;


          /**
           * Cache to insert, modify and delete client data.
           */
          static std::unique_ptr<granada::cache::CacheHandler> cache_;


          /**
           * Cryptograph to encrypt and decrypt client credentials.
           */
          static std::unique_ptr<granada::crypto::Cryptograph> cryptograph_;


          /**
           * Nonce string generator, for generating unique strings tokens.
           * Generate a nonce string containing random alphanumeric characters (A-Za-z0-9).
           */
          static std::unique_ptr<granada::crypto::NonceGenerator> n_generator_;
      };


      class MapOAuth2Code : public OAuth2Code{
        public:
          /**
           * Constructor
           * Initialize nonce generator and load properties.
           */
          MapOAuth2Code(){
            MapOAuth2Code::load_properties_call_once_.call([this](){
              this->LoadProperties();
            });
          };


          /**
           * Constructor
           * Initialize nonce generator, load properties and load code with the given code.
           */
          MapOAuth2Code(const std::string& code){
            MapOAuth2Code::load_properties_call_once_.call([this](){
              this->LoadProperties();
            });
            code_ = code;
            Load();
          };

          // override
          virtual granada::cache::CacheHandler* cache() override {
            return cache_.get();
          };

          // override
          virtual granada::crypto::Cryptograph* cryptograph() override {
            return cryptograph_.get();
          };

          // override
          virtual granada::crypto::NonceGenerator* nonce_generator() override {
            return n_generator_.get();
          };


        private:

          /**
           * Used for loading the properties only once.
           */
          static granada::util::mutex::call_once load_properties_call_once_;


          /**
           * Cache to insert, modify and delete client data.
           */
          static std::unique_ptr<granada::cache::CacheHandler> cache_;


          /**
           * Cryptograph to encrypt and decrypt client credentials.
           */
          static std::unique_ptr<granada::crypto::Cryptograph> cryptograph_;


          /**
           * Nonce string generator, for generating unique strings tokens.
           * Generate a nonce string containing random alphanumeric characters (A-Za-z0-9).
           */
          static std::unique_ptr<granada::crypto::NonceGenerator> n_generator_;
      };


      class MapOAuth2Authorization : public OAuth2Authorization{
        public:

          MapOAuth2Authorization(){
            MapOAuth2Authorization::load_properties_call_once_.call([this](){
              this->LoadProperties();
            });
          };


          MapOAuth2Authorization(const granada::http::oauth2::OAuth2Parameters& oauth2_parameters,
                                    granada::http::session::SessionFactory* session_factory){
            oauth2_parameters_ = oauth2_parameters;
            session_factory_ = session_factory;
            MapOAuth2Authorization::load_properties_call_once_.call([this](){
              this->LoadProperties();
            });
          };

          // override
          granada::cache::CacheHandler* cache() override {
            return cache_.get();
          };


        protected:

          virtual granada::http::oauth2::OAuth2Factory* factory() override {
            return oauth2_factory_.get();
          };


        private:

          /**
           * Used for loading the properties only once.
           */
          static granada::util::mutex::call_once load_properties_call_once_;


          /**
           * OAuth 2.0 Factory.
           * Used to instanciate OAuth 2.0 clients, users and codes.
           */
          static std::unique_ptr<granada::http::oauth2::OAuth2Factory> oauth2_factory_;


          /**
           * Cache to insert, modify and delete client data.
           */
          static std::unique_ptr<granada::cache::CacheHandler> cache_;


          
      };


      class MapOAuth2Factory : public OAuth2Factory{

        public:

          virtual std::unique_ptr<granada::http::oauth2::OAuth2Client>OAuth2Client_unique_ptr(){
            return granada::util::memory::make_unique<granada::http::oauth2::MapOAuth2Client>();
          };

          virtual std::unique_ptr<granada::http::oauth2::OAuth2Client>OAuth2Client_unique_ptr(const std::string& client_id){
            return granada::util::memory::make_unique<granada::http::oauth2::MapOAuth2Client>(client_id);
          };

          virtual std::unique_ptr<granada::http::oauth2::OAuth2User>OAuth2User_unique_ptr(){
            return granada::util::memory::make_unique<granada::http::oauth2::MapOAuth2User>();
          };

          virtual std::unique_ptr<granada::http::oauth2::OAuth2User>OAuth2User_unique_ptr(const std::string& username){
            return granada::util::memory::make_unique<granada::http::oauth2::MapOAuth2User>(username);
          };

          virtual std::unique_ptr<granada::http::oauth2::OAuth2Code>OAuth2Code_unique_ptr(){
            return granada::util::memory::make_unique<granada::http::oauth2::MapOAuth2Code>();
          };

          virtual std::unique_ptr<granada::http::oauth2::OAuth2Code>OAuth2Code_unique_ptr(const std::string& code){
            return granada::util::memory::make_unique<granada::http::oauth2::MapOAuth2Code>(code);
          };

          virtual std::unique_ptr<granada::http::oauth2::OAuth2Authorization>OAuth2Authorization_unique_ptr(){
            return granada::util::memory::make_unique<granada::http::oauth2::MapOAuth2Authorization>();
          };

          virtual std::unique_ptr<granada::http::oauth2::OAuth2Authorization>OAuth2Authorization_unique_ptr(const granada::http::oauth2::OAuth2Parameters& oauth2_parameters,
                                                                                                 granada::http::session::SessionFactory* session_factory){
            return granada::util::memory::make_unique<granada::http::oauth2::MapOAuth2Authorization>(oauth2_parameters,session_factory);
          };

      };

    }

  }

}
