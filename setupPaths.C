


void setupPaths() {
  const char* anitaUtilInstallDir = getenv("ANITA_UTIL_INSTALL_DIR");
  TString incDir = TString::Format("/usr/local/include");
  gInterpreter->AddIncludePath(incDir.Data());

  //gSystem->AddIncludePath("-I${ANITA_UTIL_INSTALL_DIR}/include");
  gSystem->Load("libRootFftwWrapper.3");  
}
