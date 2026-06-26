#include "Input.h"


//キー情報取得
bool Input::getKey(uint16_t sKey)const noexcept {
	return (keyState_[sKey] & 0x80);
}

//キー情報取得
bool Input::getTrigger(uint16_t sKey)const noexcept {
	return ((keyState_[sKey] & 0x80) && !(prevKeyState_[sKey] & 0x80));
}

//キー情報更新
void Input::updateKeyState(void* pState)noexcept {
	memcpy_s(keyState_.data(), keyState_.size(), pState, keyState_.size());
}
//キー情報更新
void Input::updatePrevKeyState()noexcept {
	memcpy_s(prevKeyState_.data(), prevKeyState_.size(), keyState_.data(), keyState_.size());
}