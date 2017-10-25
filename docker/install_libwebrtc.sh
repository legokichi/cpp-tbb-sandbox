#!/bin/bash
set -eu

# https://qiita.com/alivelime/items/f1447570eb96eb101cf4

cd /opt
git clone --recursive --depth 1 https://chromium.googlesource.com/chromium/tools/depot_tools.git
export PATH=$PATH:/opt/depot_tools
cd /opt/webrtc-checkout
fetch --nohooks webrtc
cd /opt/webrtc-checkout/src
git checkout -b libwebrtc59 refs/remotes/branch-heads/59
gclient sync --with_branch_heads
gn gen out/Release --args='rtc_use_h264=true rtc_use_lto=true'
ninja -C out/Release
