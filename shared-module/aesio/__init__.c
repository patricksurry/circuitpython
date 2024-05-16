/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 by Sean Cross
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
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <string.h>

#include "py/runtime.h"

#include "shared-bindings/aesio/__init__.h"
#include "shared-module/aesio/__init__.h"

void common_hal_aesio_aes_construct(aesio_aes_obj_t *self, const uint8_t *key,
    uint32_t key_length, const uint8_t *iv,
    int mode, int counter) {
    self->mode = mode;
    self->counter = counter;
    common_hal_aesio_aes_rekey(self, key, key_length, iv);
}

void common_hal_aesio_aes_rekey(aesio_aes_obj_t *self, const uint8_t *key,
    uint32_t key_length, const uint8_t *iv) {
    memset(&self->ctx, 0, sizeof(self->ctx));
    if (iv != NULL) {
        AES_init_ctx_iv(&self->ctx, key, key_length, iv);
    } else {
        AES_init_ctx(&self->ctx, key, key_length);
    }
}

void common_hal_aesio_aes_set_mode(aesio_aes_obj_t *self, int mode) {
    self->mode = mode;
}

void common_hal_aesio_aes_encrypt(aesio_aes_obj_t *self, uint8_t *buffer,
    size_t length) {
    switch (self->mode) {
        case AES_MODE_ECB:
            AES_ECB_encrypt(&self->ctx, buffer);
            break;
        case AES_MODE_CBC:
            AES_CBC_encrypt_buffer(&self->ctx, buffer, length);
            break;
        case AES_MODE_CTR:
            AES_CTR_xcrypt_buffer(&self->ctx, buffer, length);
            break;
    }
}

void common_hal_aesio_aes_decrypt(aesio_aes_obj_t *self, uint8_t *buffer,
    size_t length) {
    switch (self->mode) {
        case AES_MODE_ECB:
            AES_ECB_decrypt(&self->ctx, buffer);
            break;
        case AES_MODE_CBC:
            AES_CBC_decrypt_buffer(&self->ctx, buffer, length);
            break;
        case AES_MODE_CTR:
            AES_CTR_xcrypt_buffer(&self->ctx, buffer, length);
            break;
    }
}
