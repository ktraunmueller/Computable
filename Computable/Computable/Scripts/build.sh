#!/bin/sh
#
# Command line build script
#
# TARGET contains the target name (defaults to "Computable")
# CONFIGURATION is the build configuration, ("AdHoc" or "AppStore", defaults to "AdHoc")
#
# Note: this script has to be run in the project's root directory

TARGET=${TARGET-"Computable"}
PRODUCT="Computable"
CONFIGURATION=${CONFIGURATION-"AdHoc"}
PLIST="Computable-Info.plist"

# get app version
VERSION=$(/usr/libexec/PlistBuddy -c "Print :CFBundleShortVersionString" $PLIST)

# increment build number
BUILD_NR=$(/usr/libexec/PlistBuddy -c "Print :CFBuildNumber" $PLIST)
BUILD_NR=$(($BUILD_NR + 1))
/usr/libexec/PlistBuddy -c "Set :CFBuildNumber $BUILD_NR" $PLIST

# write bundle version
/usr/libexec/PlistBuddy -c "Set :CFBundleVersion $VERSION.$BUILD_NR" $PLIST

echo "Building $TARGET $VERSION build $BUILD_NR ($CONFIGURATION)..."

# print sdk info
echo "using SDK:"
xcodebuild -version -sdk iphoneos

# clean & build
echo "Running xcodebuild..."
xcodebuild -workspace "../Computable.xcworkspace" -scheme "$TARGET" -configuration "$CONFIGURATION" clean build >| xcodebuild_output

# locate output folders
WORKSPACE="Computable"
devired_data_path="$HOME/Library/Developer/Xcode/DerivedData"
project_derived_data_directory=$(grep -oE "$WORKSPACE-([a-zA-Z0-9]+)[/]" xcodebuild_output | sed -n "s/\($WORKSPACE-[a-z]\{1,\}\)\//\1/p" | head -n1)
project_derived_data_path="$devired_data_path/$project_derived_data_directory"
rm xcodebuild_output

# prepare for signing
ARCHIVE=$PRODUCT
SIGNER="iPhone Distribution: Karl Traunmueller"
MOBILEPROVISION="ProvisioningProfiles/KarlTraunmueller_WildcardAdHoc.mobileprovision"
echo "using profile $MOBILEPROVISION"

# copy dSYM files to the archives folder
echo "Saving .dSYM to Archives/${VERSION}..."
mkdir -p "Archives/${VERSION}"
rm -Rf "Archives/${VERSION}/${ARCHIVE}.app"
rm -Rf "Archives/${VERSION}/${ARCHIVE}.app.dSYM"
rm -Rf "Archives/${VERSION}/${ARCHIVE}.ipa"

cp -Rf "$project_derived_data_path/Build/Products/${CONFIGURATION}-iphoneos/${PRODUCT}.app" "Archives/${VERSION}/${ARCHIVE}.app"
cp -Rf "$project_derived_data_path/Build/Products/${CONFIGURATION}-iphoneos/${PRODUCT}.app.dSYM" "Archives/${VERSION}/${ARCHIVE}.app.dSYM"

# create dSYM.zip for HockeyApp
rm -f "Archives/${VERSION}/${ARCHIVE}.app.dSYM.zip"
zip -r "Archives/${VERSION}/${ARCHIVE}.app.dSYM.zip" "Archives/${VERSION}/${ARCHIVE}.app.dSYM"

# create and sign AdHoc ipa
echo "Signing app using identity $SIGNER / profile $MOBILEPROVISION..."
xcrun -sdk iphoneos PackageApplication \
    -v "$(pwd)/Archives/${VERSION}/${ARCHIVE}.app" \
    -o "$(pwd)/Archives/${VERSION}/${ARCHIVE}.ipa" \
    --sign "$SIGNER" \
    --embed "$MOBILEPROVISION"

# re-sign for app store distribution
ARCHIVE=$PRODUCT
SIGNER="iPhone Distribution: Karl Traunmueller"
MOBILEPROVISION="$(pwd)/ProvisioningProfiles/${PRODUCT}AppStore.mobileprovision"

CWD=$(pwd)
cd "Archives/${VERSION}"
unzip "${ARCHIVE}.ipa"
cp "${CWD}/Entitlements.plist" "Payload/${PRODUCT}.app/Entitlements.plist"
rm -r "Payload/${PRODUCT}.app/_CodeSignature" "Payload/${PRODUCT}.app/CodeResources" 2> /dev/null | true
cp "${MOBILEPROVISION}" "Payload/${PRODUCT}.app/embedded.mobileprovision"
/usr/bin/codesign -f -s "$SIGNER" \
    --resource-rules \
    "Payload/${PRODUCT}.app/ResourceRules.plist" \
    --entitlements "Payload/${PRODUCT}.app/Entitlements.plist" \
    "Payload/${PRODUCT}.app"
zip -qr "${ARCHIVE}-AppStore.ipa" Payload
rm -rf Payload
cd $CWD

# clean up
rm -Rf "Archives/${VERSION}/${ARCHIVE}.app"

# upload to HockeyApp
HOCKEYAPP_API_TOKEN="--REMOVED--"

echo "Uploading to HockeyApp..."
/usr/bin/curl "https://rink.hockeyapp.net/api/2/apps" \
    -F status="2" \
    -F ipa=@"Archives/${VERSION}/${ARCHIVE}.ipa" \
    -F dsym=@"Archives/${VERSION}/${ARCHIVE}.app.dSYM.zip" \
    -F notes="${PRODUCT} ${VERSION} build ${BUILD_NR}" \
    -H "X-HockeyAppToken: $HOCKEYAPP_API_TOKEN"

echo "Finished uploading ${PRODUCT} ${VERSION} build ${BUILD_NR}"

git commit -a -m "$TARGET $VERSION build $BUILD_NR"
git push
git tag -a -f ${TARGET}-${VERSION}-${BUILD_NR} -m "$TARGET $VERSION build $BUILD_NR"
git push --tags
