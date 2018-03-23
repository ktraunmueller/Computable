rm *.strings
find . \( -name "*.m" -o -name "*.h" -o -name "*.mm" \) -print0 | xargs -0 genstrings

rm AFNetworking.strings
rm CargoBay.strings

cp Localizable.strings Computable.strings
mv -f Localizable.strings Resources/Localizations/en.lproj
