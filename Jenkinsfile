pipeline {

  agent any
  tools {
        maven "Maven"
    }

  parameters{
      choice(name:'BuildType', choices:'Debug\nRelease\nRelWithDebInfo',description:'Which type of build to consider?'
          )
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
        stage('Load Dependencies')
        {
          parallel
          {
            stage("Linux Platform")
            {
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

                  sh 'echo "create conan profile..."'
                  sh 'conan profile detect --force'

                  sh 'echo "Loading..."'
                  script {
                              if ( ${params.BuildType} == 'RelWithDebInfo') {
                                sh """conan install .  --build=missing  -s "&:build_type=${params.BuildType}" -sbuild_type=Release"""
                              }
                              else {
                                sh """conan install . -sbuild_type=${params.BuildType} --build=missing"""
                              }
                          }

                }
            }

            stage("Windows Platform")
            {
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

                  bat 'echo "create conan profile..."'
                  bat 'conan profile detect --force'

                  echo "Loading..."
                  script {

                                  bat "echo --------------------------${env.CHANGE_ID}"
                                  bat "echo -------------------------- ${params.BuildType}"

                              if ( ${params.BuildType} == 'RelWithDebInfo') {
                                bat """conan install .  --build=missing  -s "&:build_type=${params.BuildType}" -sbuild_type=Release"""
                              }
                              else {
                                bat """conan install . -sbuild_type=${params.BuildType} --build=missing"""
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
              agent { label 'Docker-Agent-Linux'}
              steps
              {
                sh 'echo "Configuring..."'
                sh 'cmake --preset conan-$buildTypeLower'
              }
            }
            stage("Windows Platform")
            {
              agent { label 'Physical-Agent-Windows'}
              steps
              {
                bat 'echo "Configuring..."'
                bat 'cmake --preset conan-default'
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
              agent { label 'Docker-Agent-Linux'}
              steps
              {
                sh 'echo "Building..."'
                sh 'cmake --build --parallel 4  --preset conan-$buildTypeLower'
              }
            }
            stage("Windows Platform")
            {
              agent { label 'Physical-Agent-Windows'}
              steps
              {
                bat 'echo "Building..."'
                bat 'cmake --build --parallel 4  --preset conan-%buildTypeLower%'
              }
            }
          }
        }

        stage('Run UnitTests')
        {
          parallel
          {
            stage("Linux Platform")
            {
              agent { label 'Docker-Agent-Linux'}
              steps {
                sh 'echo "Running Unit Tests..."'
                sh 'ctest -V --build-config $BuildType --test-dir build/$BuildType --output-junit  unitTestReports.xml'

                sh 'echo "Running Coverage Tests..."'
                sh 'ctest -V -T Coverage --test-dir build/$BuildType'
                sh 'gcovr -r build/$BuildType --cobertura-pretty --cobertura --exclude-unreachable-branches --exclude-throw-branches --print-summary --root . --output coverageTestsReports.xml'
              }
              post {
                success  {
                    junit (testResults:'build/$BuildType/unitTestReports.xml', allowEmptyResults : true)
                }
              }
            }

            stage("Windows Platform")
            {
              agent { label 'Physical-Agent-Windows'}
              steps {
                bat 'echo "Running Unit Tests..."'
                bat 'ctest -V --build-config %BuildType% --test-dir build'
              }
              post {
                success  {
                    junit (testResults:'build/%BuildType%/unitTestReports.xml', allowEmptyResults : true)
                }
              }

            }
          }
        }

        // stage('SonarQube')
        // {
        //   agent { label 'Docker-Agent-Linux'}
        //   steps
        //   {
        //     sh 'echo "Static C/C++ code analysis ===> CPPCHECK"'
        //     sh 'cppcheck \
        //         --enable=all  \
        //         -v \
        //         --language=c++ \
        //         --suppress=missingIncludeSystem \
        //         --inconclusive \
        //         --std=c++17 \
        //         --platform=win64 \
        //         --xml \
        //         --xml-version=2  src 2> cppcheck.xml'

        //     sh 'echo "Static C/C++ code analysis ===> RATS"'
        //     sh 'rats \
        //         -w 3 \
        //         --xml \
        //         -l "c" \
        //         src > rats.xml'


        //     sh 'echo "Static C/C++ code analysis ===> CLANG"'
        //     sh 'cd build/$BuildType && scan-build \
        //         -plist \
        //         -analyze-headers \
        //         -o clang_reports make'

        //     sh 'echo "Static C/C++ code analysis ===> CLANG-TIDY"'
        //     catchError(buildResult: 'SUCCESS', stageResult: 'SUCCESS', message: 'Skip in case of cmd failure')
        //     {
        //       sh 'cd build/$BuildType && run-clang-tidy > clang-tidy.txt'
        //     }

        //     script
        //     {
        //         withSonarQubeEnv('sonarqube_cpplib') {
        //           sh "/opt/sonar-scanner/bin/sonar-scanner \
        //               -Dsonar.sources=src \
        //               -Dsonar.projectKey=cpplib \
        //               -Dsonar.cfamily.compile-commands=build/$BuildType/compile_commands.json \
        //               -Dsonar.cxx.includeDirectories=src/Utilities/Common/headers,src/Utilities/Logger/headers,src/Utilities/Network/headers \
        //               -Dsonar.cxx.cppcheck.reportPaths=cppcheck.xml \
        //               -Dsonar.cxx.rats.reportPaths=rats.xml \
        //               -Dsonar.cxx.clangsa.reportPaths=build/$BuildType/clang_reports/*/*.plist \
        //               -Dsonar.cxx.clangtidy.reportPaths=build/$BuildType/clang-tidy.txt \
        //               -Dsonar.cxx.xunit.reportPaths=build/$BuildType/unitTestReports.xml \
        //               -Dsonar.cxx.cobertura.reportPaths=coverageTestsReports.xml  \
        //               -Dsonar.qualitygate.wait=true \
        //               -Dsonar.qualitygate.timeout=300 \
        //               -Dsonar.branchname=${env.BRANCH_NAME} \
        //               -Dsonar.verbose=true "
        //         }
        //     }
        //   }
        // }

        stage('Package')
        {
          parallel
          {
            stage("Linux Platform")
            {
              agent { label 'Docker-Agent-Linux'}
              steps
              {
                sh 'echo "Packaging..."'
                sh 'cd  build/$BuildType && make package'
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
              agent { label 'Physical-Agent-Windows'}
              steps
              {
                bat 'echo "Packaging..."'
                bat 'cd  build && cpack -C $BuildType'
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
              agent { label 'Docker-Agent-Linux'}
                steps {
                    script {
                      if (BuildType == 'Release') {
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
                                version: "Linux-${BuildType}-${versionExtracted}",
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
              agent { label 'Physical-Agent-Windows'}
              steps
               {
                  script
                  {
                    if (BuildType == 'Release') {
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
                              version: "Windows-${BuildType}-${versionExtracted}",
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
