var path = require( 'path' )

const HtmlWebPackPlugin = require("html-webpack-plugin");

const htmlPlugin = new HtmlWebPackPlugin({
  template: "./public/index.html",
  filename: "./index.html"
});

module.exports = {
  resolve: {
    extensions: ['.js', '.jsx'],
    alias: {
      src: path.resolve( __dirname, 'src/' )
    }
  },
  output: {
    filename: 'bundle.js',
    path: path.join( __dirname, 'build/' )
  },
  module: {
    rules: [
      {
        test: /\.js$/,
        exclude: /node_modules/,
        use: {
          loader: "babel-loader"
        }
      }
    ]
  },
  plugins: [htmlPlugin]
};
