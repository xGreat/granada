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
  * Encrypt and decrypt using openssl AES
  *
  */

#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cryptograph.h"
#include <openssl/aes.h>

namespace granada{
  namespace crypto{
    class OpensslAESCryptograph : public Cryptograph{
      public:

        /**
         * Constructor
         */
        OpensslAESCryptograph(){};


        /**
         * Destructor
         */
        virtual ~OpensslAESCryptograph(){};


        // override
        std::string Encrypt(const std::string& text, std::string password) override {
          password += "qwertyuiopasdfghjklzxcvbnmqwertyqwertyuiopasdfghjklzxcvbnmqwerty";
		  
		  const int text_size = text.size() + 1;
		  unsigned char *text_to_encrypt = new unsigned char[text_size]();
          memcpy(text_to_encrypt,text.c_str(),text.size()+1);

		  const int key_size = password.size() + 1;
		  unsigned char *key = new unsigned char[key_size]();

          memcpy(key,password.c_str(),password.size()+1);
		  unsigned char enc_out[256];
          AES_KEY enc_key;
          AES_set_encrypt_key(key, 256, &enc_key);
          AES_encrypt(text_to_encrypt, enc_out, &enc_key);

		  delete[] text_to_encrypt;
		  delete[] key;
          return std::string(reinterpret_cast<const char*>(enc_out),256);
        };

        // override
        std::string Decrypt(const std::string& text, std::string password) override {
          password += "qwertyuiopasdfghjklzxcvbnmqwertyqwertyuiopasdfghjklzxcvbnmqwerty";

		  const int key_size = password.size() + 1;
		  unsigned char *key = new unsigned char[key_size]();
          memcpy(key,password.c_str(),password.size()+1);

          unsigned char dec_out[256];
          AES_KEY dec_key;
          AES_set_decrypt_key(key,256,&dec_key);

          unsigned char crypted_text[256];
          memcpy(crypted_text,text.c_str(),256);
          AES_decrypt(crypted_text, dec_out, &dec_key);

		  delete[] key;

          return std::string(reinterpret_cast<const char*>(dec_out));
        };

    };
  }
}
