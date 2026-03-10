pipeline {

  agent any


  parameters{

      //Enable Valgrind
      booleanParam(name:'Valgrind', defaultValue: false, description:'Enable Valgrind for profiling Memory/Leak')

      //Enable Documentation
      booleanParam(name:'Documentation', defaultValue: false, description:'Enable Doc Generation')

      //Enable Run Units Tests
      booleanParam(name:'UnitsTests', defaultValue: true, description:'Enable UnitsTests Run')

      //Build With CLANG
      booleanParam(name:'Clang', defaultValue: false, description:'Use Clang compiler for Linux platform')

      //Build Platform Selection
      booleanParam(name:'LinuxBuild', defaultValue: true, description:'Enable Linux build stages')
      booleanParam(name:'WindowsBuild', defaultValue: true, description:'Enable Windows build stages')

      //Build Type
      //The FIRST item in the choices array becomes the default
      choice(name:'BuildType', choices:'Debug\nRelease\nRelWithDebInfo',description:'Which type of build to consider?')
  }

  environment
  {
              buildTypeLower = "${BuildType.toLowerCase()}"
              NEXUS_VERSION = "nexus3"
              NEXUS_PROTOCOL = "http"
              NEXUS_URL = "172.17.0.1:8081"
              NEXUS_REPOSITORY = "CppLib"
              NEXUS_CREDENTIAL_ID = "nexus_cpplib"
  }

  stages
  {
        stage('Validate Platform Selection')
        {
          steps {
            script {
              if (params.LinuxBuild && !params.WindowsBuild) {
                echo "🟠WARNING: Windows build is disabled. This run will build/test/package only for Linux."
              } else if (!params.LinuxBuild && params.WindowsBuild) {
                echo "🟠WARNING: Linux build is disabled. This run will build/test/package only for Windows."
              }

              if (!params.LinuxBuild && !params.WindowsBuild) {
                error "🔴Error: At least one platform (Linux or Windows) must be enabled."
              }
            }
          }
        }

        stage('Load Dependencies')
        {
          parallel
          {
            stage("Linux Platform")
            {
                when { expression { params.LinuxBuild } }
                agent { label 'Docker-Agent-Linux'}
                steps
                {
                  echo 'Clean up...'
                  sh 'if [ -d build ]; then rm -rf build ;fi'
                  sh 'if [ -d _PACKAGE_ ]; then rm -rf _PACKAGE_;fi'

                  script {
                              currentBuild.displayName = "[#${BUILD_NUMBER}]"
                              currentBuild.description = "Build Type: ${params.BuildType}\n"  +
                                                        "Branch Name: ${env.BRANCH_NAME}\n" +
                                                        "Executed on: ${NODE_NAME}\n"
                          }

                  sh 'echo "Load conan profile..."'
                  script {
                            def compilerProfile = params.Clang ? "ConanProfiles/ClangProfile" : "ConanProfiles/GccProfile"
                            if (params.BuildType== 'RelWithDebInfo') {
                              sh """conan install .  --build=missing  -s "&:build_type=${params.BuildType}" -sbuild_type=Release --profile=${compilerProfile}"""
                            }
                            else {
                              sh """conan install . -sbuild_type=${params.BuildType} --build=missing  --profile=${compilerProfile}"""
                            }
                          }

                }
            }

            stage("Windows Platform")
            {
              when { expression { params.WindowsBuild } }
              agent { label 'Physical-Agent-Windows'}
              steps
                {
                  echo 'Clean up...'
                  bat 'if exist build (rmdir build /S /Q)'
                  bat 'if exist _PACKAGE_ (rmdir _PACKAGE_ /S /Q)'

                  script {
                              currentBuild.displayName = "[#${BUILD_NUMBER}]"
                              currentBuild.description = "Build Type: ${params.BuildType}\n"  +
                                                        "Branch Name: ${env.BRANCH_NAME}\n" +
                                                        "Executed on: ${NODE_NAME}\n"
                          }

                  echo "Load conan profile..."
                  script {
                              if (params.BuildType == 'RelWithDebInfo') {
                                bat """conan install .  --build=missing  -s "&:build_type=${params.BuildType}" -sbuild_type=Release --profile=ConanProfiles/MsvcProfile"""
                              }
                              else {
                                bat """conan install . -sbuild_type=${params.BuildType} --build=missing  --profile=ConanProfiles/MsvcProfile"""
                              }
                          }
                }
            }
          }
        }

        stage('Configure')
        {
          parallel
          {
            stage("Linux Platform")
            {
              when { expression { params.LinuxBuild } }
              agent { label 'Docker-Agent-Linux'}
              steps
              {
                sh 'echo "Configuring..."'
                script
                {
                  def buildOptions = " "
                  if (params.Valgrind) {
                    buildOptions += " -DBUILD_WITH_MEMCHECK_VAL=ON "
                  }

                  if (params.Documentation) {
                    buildOptions += " -DBUILD_WITH_DOC=ON "
                  }

                  if (params.Clang) {
                    buildOptions += " -DBUILD_WITH_CLANG=ON "
                  }
                  sh "cmake  ${buildOptions} --preset conan-$buildTypeLower"

                }
              }
            }
            stage("Windows Platform")
            {
              when { expression { params.WindowsBuild } }
              agent { label 'Physical-Agent-Windows'}
              steps
              {
                bat 'echo "Configuring..."'
                script
                {
                  def buildOptions = " "
                  if (params.Documentation) {
                    buildOptions += " -DBUILD_WITH_DOC=ON "
                  }
                  bat "cmake  ${buildOptions} --preset conan-default"
                }

              }
            }
          }
        }

        stage('Build')
        {
          parallel
          {
            stage("Linux Platform")
            {
              when { expression { params.LinuxBuild } }
              agent { label 'Docker-Agent-Linux'}
              steps
              {
                sh 'echo "Building..."'
                sh 'cmake --build --parallel 4  --preset conan-$buildTypeLower | tee lin_build.log'
              }
            }
            stage("Windows Platform")
            {
              when { expression { params.WindowsBuild } }
              agent { label 'Physical-Agent-Windows'}
              steps
              {
                bat 'echo "Building..."'
                bat 'powershell "cmake --build --parallel 4  --preset conan-%buildTypeLower% |  tee win_build.log"'
                stash includes: 'win_build.log', name: 'win_build'
              }
            }
          }
        }

        stage('Run UnitTests')
        {
          when {
                expression { params.UnitsTests }
            }
          parallel
          {
            stage("Linux Platform")
            {
              when { expression { params.LinuxBuild } }
              agent { label 'Docker-Agent-Linux'}
              steps {
                sh 'echo "Running Unit Tests..."'
                sh """ctest -V --build-config ${params.BuildType} --test-dir build/${params.BuildType} --output-junit  unitTestReports.xml"""

                script
                {
                    sh 'echo "Running Coverage Tests..."'
                    if (params.Clang)
                    {
                      sh "export LLVM_PROFILE_FILE=\"\${PWD}/build/${params.BuildType}/Profraw/test_%p.profraw\" && ctest --test-dir build/${params.BuildType}"
                      sh """
                        llvm-profdata merge -sparse build/${params.BuildType}/Profraw/*.profraw -o coverageTests.profdata
                        # start fresh lcov output for executables
                        > lcov_exec.info
                        # loop over each unit test binary and append its coverage data
                        for exe in \$(find . -type f -executable -name "*_test"); do
                            echo "Exporting coverage for \$exe"
                            llvm-cov export --format=lcov \\
                                --instr-profile=coverageTests.profdata \\
                                --ignore-filename-regex=".*/unitTest/.*" \\
                                "\$exe" >> lcov_exec.info
                        done
                        """                      
                      sh """llvm-cov export --format=lcov --instr-profile=coverageTests.profdata   \$(find \${PWD} -name "*.so") > lcov_lib.info"""
                      sh """lcov -a lcov_exec.info -a lcov_lib.info -o coverageTestsReports.info"""
                      sh """lcov_cobertura coverageTestsReports.info --output coverageTestsReports.xml"""
                    }
                    else
                    {
                      sh """ctest -V -T Coverage --test-dir build/${params.BuildType}"""
                      sh """gcovr --root src/ --filter src/ --exclude-directories .*/unitTest  --object-directory build/${params.BuildType} --cobertura-pretty --cobertura --print-summary  --output coverageTestsReports.xml"""
                    }
                }

              }
              post {
                success  {
                    junit (testResults:"""build/${params.BuildType}/unitTestReports.xml""", allowEmptyResults : true)
                }
              }
            }

            stage("Windows Platform")
            {
              when { expression { params.WindowsBuild } }
              agent { label 'Physical-Agent-Windows'}
              steps {
                bat 'echo "Running Unit Tests..."'
                bat """ctest -V --build-config ${params.BuildType} --test-dir build  --output-junit  unitTestReports.xml"""
              }
              post {
                success  {
                    junit (testResults:"""build/${params.BuildType}/unitTestReports.xml""", allowEmptyResults : true)
                }
              }

            }
          }
        }

        stage('SonarQube')
        {
              when { expression { params.LinuxBuild } }
              agent { label 'Docker-Agent-Linux'}
              environment { ScannerHomePath = tool 'SonarScanner'}
              steps
              {
                sh 'echo "Static C/C++ code analysis ===> CPPCHECK"'
                sh """cppcheck \
                    --enable=all  \
                    -j 4 \
                    -v \
                    --language=c++ \
                    --suppress=missingIncludeSystem \
                    --inconclusive \
                    --std=c++17 \
                    --platform=win64 \
                    --xml \
                    --xml-version=2 \
                    --project=build/${params.BuildType}/compile_commands.json \
                    2> cppcheck.xml"""

                sh 'echo "Static C/C++ code analysis ===> RATS"'
                sh 'rats \
                    -w 3 \
                    --xml \
                    -l "c" \
                    src > rats.xml'


                sh 'echo "Static C/C++ code analysis ===> CLANG"'
                sh """cd build/${params.BuildType} && scan-build \
                    -plist \
                    -analyze-headers \
                    -o clang_reports make"""

                sh 'echo "Static C/C++ code analysis ===> CLANG-TIDY"'
                catchError(buildResult: 'SUCCESS', stageResult: 'SUCCESS', message: 'Skip in case of cmd failure')
                {
                  sh """cd build/${params.BuildType} && run-clang-tidy > clang-tidy.txt"""
                }

                script
                {               
                    def sonarArgs = [
                          "-Dsonar.sources=src",
                          "-Dsonar.projectKey=cpplib",
                          "-Dsonar.exclusions=src/***/unitTest/*.cpp",
                          "-Dsonar.cfamily.compile-commands=build/${params.BuildType}/compile_commands.json",
                          "-Dsonar.cxx.includeDirectories=src/Utilities/Common/headers,src/Utilities/Logger/headers,src/Utilities/Network/headers",
                          "-Dsonar.cxx.gcc.encoding=UTF-8 ",
                          "-Dsonar.cxx.gcc.reportPaths=lin_build.log",
                          "-Dsonar.cxx.vc.encoding=UTF-8",
                          "-Dsonar.cxx.valgrind.reportPaths=build/${params.BuildType}/**/*_test_valgrind.xml",
                          "-Dsonar.cxx.cppcheck.reportPaths=cppcheck.xml",
                          "-Dsonar.cxx.rats.reportPaths=rats.xml",
                          "-Dsonar.cxx.clangsa.reportPaths=build/${params.BuildType}/clang_reports/*/*.plist",
                          "-Dsonar.cxx.clangtidy.reportPaths=build/${params.BuildType}/clang-tidy.txt",
                          "-Dsonar.cxx.xunit.reportPaths=build/${params.BuildType}/unitTestReports.xml",
                          "-Dsonar.cxx.cobertura.reportPaths=coverageTestsReports.xml",
                          "-Dsonar.qualitygate.wait=true",
                          "-Dsonar.qualitygate.timeout=300",
                          "-Dsonar.branchname=${env.BRANCH_NAME}",
                          "-Dsonar.verbose=true" 
                    ]

                    if (params.WindowsBuild) {                    
                      unstash 'win_build'                    
                      sonarArgs << "-Dsonar.cxx.vc.reportPaths=win_build.log"
                    }                   

                    withSonarQubeEnv('sonarqube_cpplib') {
                      sh "${ScannerHomePath}/bin/sonar-scanner ${sonarArgs.join(' ')}"
                    }
                }
              }
        }

        stage('Package')
        {
          parallel
          {
            stage("Linux Platform")
            {
              when { expression { params.LinuxBuild } }
              agent { label 'Docker-Agent-Linux'}
              steps
              {
                sh 'echo "Packaging..."'
                sh """cd  build/${params.BuildType} && make package"""
              }
              post
              {
                success
                {
                    archiveArtifacts artifacts: '_PACKAGE_/CppLib-*.sh', onlyIfSuccessful: true
                }
              }
            }
            stage("Windows Platform")
            {
              when { expression { params.WindowsBuild } }
              agent { label 'Physical-Agent-Windows'}
              steps
              {
                bat 'echo "Packaging..."'
                bat """cd  build && cpack -C ${params.BuildType}"""
              }
              post
              {
                success
                {
                    archiveArtifacts artifacts: '_PACKAGE_/CppLib-*.exe', onlyIfSuccessful: true
                }
              }
            }
          }
        }

        stage("Publish to nexus")
        {
          parallel
          {
            stage("Linux Platform")
            {
              when { expression { params.LinuxBuild } }
              agent { label 'Docker-Agent-Linux'}
                steps {
                    script {
                      if (params.BuildType == 'Release') {
                        // Find built artifact under target folder
                        filesByGlob = findFiles(glob: "_PACKAGE_/*.sh");
                        // Extract the path from the File found
                        artifactPath = filesByGlob[0].path;
                        // Assign to a boolean response verifying If the artifact name exists
                        artifactExists = fileExists artifactPath;
                        //Extract current version
                        versionExtracted = artifactPath.split('-')[1]
                        if(artifactExists) {
                            nexusArtifactUploader(
                                nexusVersion: NEXUS_VERSION,
                                protocol: NEXUS_PROTOCOL,
                                nexusUrl: NEXUS_URL,
                                groupId: "org.stroalgo",
                                version: "Linux-${params.BuildType}-${versionExtracted}",
                                repository: NEXUS_REPOSITORY,
                                credentialsId: NEXUS_CREDENTIAL_ID,
                                artifacts: [
                                    [artifactId: NEXUS_REPOSITORY,
                                    classifier: 'Release',
                                    file: artifactPath,
                                    type: 'sh'
                                    ]
                                ]
                            );

                        } else {
                            error "*** File: ${artifactPath}, could not be found";
                        }
                      } else {
                        sh 'echo "Not a release build type for Publishing to Nexus"'
                      }
                    }

                }
            }
            stage("Windows Platform")
            {
              when { expression { params.WindowsBuild } }
              agent { label 'Physical-Agent-Windows'}
              steps
               {
                  script
                  {
                    if (params.BuildType == 'Release') {
                      // Find built artifact under target folder
                      filesByGlob = findFiles(glob: "_PACKAGE_/*.exe");
                      // Extract the path from the File found
                      artifactPath = filesByGlob[0].path;
                      // Assign to a boolean response verifying If the artifact name exists
                      artifactExists = fileExists artifactPath;
					  //Extract current version
                      versionExtracted = artifactPath.split('-')[1]

                      if(artifactExists) {
                          nexusArtifactUploader(
                              nexusVersion: NEXUS_VERSION,
                              protocol: NEXUS_PROTOCOL,
                              nexusUrl: "localhost:8081/",
                              groupId: "org.stroalgo",
                              version: "Windows-${params.BuildType}-${versionExtracted}",
                              repository: NEXUS_REPOSITORY,
                              credentialsId: NEXUS_CREDENTIAL_ID,
                              artifacts: [
                                  [artifactId: NEXUS_REPOSITORY,
                                  classifier: 'win64',
                                  file: artifactPath,
                                  type: 'exe'
                                  ]
                              ]
                          );

                      } else {
                          error "*** File: ${artifactPath}, could not be found";
                      }
                    } else {
                      bat 'echo "Not a release build type for Publishing to Nexus"'
                    }
                  }
                }
            }
          }
        }
  }


  post
  {
    always
    {
      emailext attachLog: true,
            body: "${currentBuild.currentResult}: Job ${env.JOB_NAME} build ${env.BUILD_NUMBER}\n More info at: ${env.BUILD_URL}",
            recipientProviders: [developers(), requestor()],
            subject: "Jenkins Build ${currentBuild.currentResult}: Job ${env.JOB_NAME}"
    }
  }
}
