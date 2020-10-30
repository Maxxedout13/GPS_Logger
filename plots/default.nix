with import <nixpkgs> {};

(let

foo="bar";

in

stdenv.mkDerivation {
  name = "impurePythonEnv";
  buildInputs = [
    qt5.full
    
    (python37.buildEnv.override {
      extraLibs = [
        pkgs.python37Packages.pyqt5        
	pkgs.python37Packages.matplotlib
        pkgs.python37Packages.docopt
        pkgs.python37Packages.gnureadline
        pkgs.python37Packages.numpy
        pkgs.python37Packages.numpy.blas
        pkgs.python37Packages.scipy
        pkgs.python37Packages.setuptools
        pkgs.python37Packages.unidecode
        pkgs.python37Packages.cartopy
      ];
      ignoreCollisions = true;
    })
  ];
  shellHook = ''
        # set SOURCE_DATE_EPOCH so that we can use python wheels
        SOURCE_DATE_EPOCH=$(date +%s)
        export LANG=en_US.UTF-8
  '';
})



#  nix-shell



















