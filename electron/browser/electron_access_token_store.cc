// Copyright (c) 2014 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "electron/browser/electron_access_token_store.h"

#include <utility>

#include "electron/browser/electron_browser_context.h"
#include "electron/browser/electron_browser_main_parts.h"
#include "electron/common/google_api_key.h"
#include "content/public/browser/geolocation_provider.h"

namespace electron {

namespace {

// Notice that we just combined the api key with the url together here, because
// if we use the standard {url: key} format Chromium would override our key with
// the predefined one in common.gypi of libchromiumcontent, which is empty.
const char* kGeolocationProviderURL =
    "https://www.googleapis.com/geolocation/v1/geolocate?key="
    GOOGLEAPIS_API_KEY;

}  // namespace

ElectronAccessTokenStore::ElectronAccessTokenStore() {
  content::GeolocationProvider::GetInstance()->UserDidOptIntoLocationServices();
}

ElectronAccessTokenStore::~ElectronAccessTokenStore() {
}

void ElectronAccessTokenStore::LoadAccessTokens(
    const LoadAccessTokensCallbackType& callback) {
  AccessTokenSet access_token_set;

  // Equivelent to access_token_set[kGeolocationProviderURL].
  // Somehow base::string16 is causing compilation errors when used in a pair
  // of std::map on Linux, this can work around it.
  std::pair<GURL, base::string16> token_pair;
  token_pair.first = GURL(kGeolocationProviderURL);
  access_token_set.insert(token_pair);

  auto browser_context = ElectronBrowserMainParts::Get()->browser_context();
  callback.Run(access_token_set, browser_context->url_request_context_getter());
}

void ElectronAccessTokenStore::SaveAccessToken(const GURL& server_url,
                                           const base::string16& access_token) {
}

}  // namespace electron