mkdir -p node_modules
npm install $LEARNUV_HOSTNAME:3000/learnuv-0.1.0.tgz
mv ./node_modules/learnuv .
rm -rf node_modules
cd learnuv
