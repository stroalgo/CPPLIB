pipeline {
  
  agent NODE_NAME
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
      parallel
      {
            stage("Build Linux Platform")
            {
                agent { label 'Docker-Agent-Linux'}
                stage('Load Dependencies') 
                {  
                  steps {

                    echo 'Clean up...'
                    sh 'if [ -d build ]; then rm -rf build;fi'

                    script {
                                currentBuild.displayName = "[#${BUILD_NUMBER}]"
                                currentBuild.description = "Build Type: ${params.BuildType}\n"  +
                                                          "Branch Name: ${env.BRANCH_NAME}\n" +                                               
                                                          "Executed on: ${NODE_NAME}\n"
                            }
                    sh 'echo "check or create conan profile..."'
                    sh 'if [ ! -f ~/.conan2/profiles/default ]; then conan profile detect; fi'
                    
                    sh 'echo "Loading..."'   
                    script {
                                if (BuildType == 'RelWithDebInfo') {
                                  sh 'conan install .  --build=missing  -s "&:build_type=$BuildType" -sbuild_type=Release'
                                }
                                else {
                                  sh 'conan install . -sbuild_type=$BuildType --build=missing'
                                }
                            }     
                  
                  }
                }

                stage('Configure') {    
                  steps {
                    sh 'echo "Configuring..."'        
                    sh 'cmake --preset conan-$buildTypeLower'       
                  }
                }

                stage('Build') {    
                  steps {
                    sh 'echo "Building..."'        
                    sh 'cmake --build --parallel 4  --preset conan-$buildTypeLower'        
                  }      
                }

                stage('Test') {
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

                stage('SonarQube') {

                  steps {
                            sh 'echo "Static C/C++ code analysis ===> CPPCHECK"'
                            sh 'cppcheck \
                                --enable=all  \
                                -v \
                                --language=c++ \
                                --suppress=missingIncludeSystem \
                                --inconclusive \
                                --std=c++17 \
                                --platform=win64 \
                                --xml \
                                --xml-version=2  src 2> cppcheck.xml'  

                            sh 'echo "Static C/C++ code analysis ===> RATS"'  
                            sh 'rats \
                                -w 3 \
                                --xml \
                                -l "c" \
                                src > rats.xml'


                            sh 'echo "Static C/C++ code analysis ===> CLANG"'  
                            sh 'cd build/$BuildType && scan-build \
                                -plist \
                                -analyze-headers \
                                -o clang_reports make'

                            sh 'echo "Static C/C++ code analysis ===> CLANG-TIDY"'  
                            catchError(buildResult: 'SUCCESS', stageResult: 'SUCCESS', message: 'Skip in case of cmd failure')
                            {
                              sh 'cd build/$BuildType && run-clang-tidy > clang-tidy.txt'
                            }

                      script {                    
                          withSonarQubeEnv('sonarqube_cpplib') {
                            sh "/opt/sonar-scanner/bin/sonar-scanner \
                                -Dsonar.sources=src \
                                -Dsonar.projectKey=cpplib \
                                -Dsonar.cfamily.compile-commands=build/$BuildType/compile_commands.json \
                                -Dsonar.cxx.includeDirectories=src/Utilities/Common/headers,src/Utilities/Logger/headers,src/Utilities/Network/headers \
                                -Dsonar.cxx.cppcheck.reportPaths=cppcheck.xml \
                                -Dsonar.cxx.rats.reportPaths=rats.xml \
                                -Dsonar.cxx.clangsa.reportPaths=build/$BuildType/clang_reports/*/*.plist \
                                -Dsonar.cxx.clangtidy.reportPaths=build/$BuildType/clang-tidy.txt \
                                -Dsonar.cxx.xunit.reportPaths=build/$BuildType/unitTestReports.xml \
                                -Dsonar.cxx.cobertura.reportPaths=coverageTestsReports.xml  \
                                -Dsonar.qualitygate.wait=true \
                                -Dsonar.qualitygate.timeout=300 \
                                -Dsonar.branchname=${env.BRANCH_NAME} \
                                -Dsonar.verbose=true " 
                          }
                      }
                  }
                    }

                stage('Package') {
                  steps {
                    sh 'echo "Packaging..."'        
                    sh 'cd  build/$BuildType && make package'        
                  }
                  post {
                    success  {
                        archiveArtifacts artifacts: '_PACKAGE_/CppLib-*.sh', onlyIfSuccessful: true
                    }
                  }
                }

                stage("Publish to nexus") {
                        steps {
                            script {
                              if (BuildType == 'Release') {
                                sh 'echo "Publish package to Nexus..."'                      
                                // Find built artifact under target folder
                                filesByGlob = findFiles(glob: "_PACKAGE_/*.sh");                    
                                // Extract the path from the File found
                                artifactPath = filesByGlob[0].path;
                                // Assign to a boolean response verifying If the artifact name exists
                                artifactExists = fileExists artifactPath;                   
                                //Extract current version
                                versionExtracted = artifactPath.split('-')[1]                    
                                if(artifactExists) {
                                    echo "*** File: ${artifactPath}";
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
                                            classifier: '',
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
                
                    
              
            }
            
            stage("Build Windows Platform")
            {
              agent { label 'Physical-Agent-Windows'}
              stage('Load Dependencies') 
              {  
                  steps 
                  {
                    echo 'Clean up...'
                    sh 'if [ -d build ]; then rm -rf build;fi'

                    script {
                                currentBuild.displayName = "[#${BUILD_NUMBER}]"
                                currentBuild.description = "Build Type: ${params.BuildType}\n"  +
                                                          "Branch Name: ${env.BRANCH_NAME}\n" +                                               
                                                          "Executed on: ${NODE_NAME}\n"
                            }
                    sh 'echo "check or create conan profile..."'
                    sh 'if [ ! -f ~/.conan2/profiles/default ]; then conan profile detect; fi'
                    
                    sh 'echo "Loading..."'   
                    script {
                                if (BuildType == 'RelWithDebInfo') {
                                  sh 'conan install .  --build=missing  -s "&:build_type=$BuildType" -sbuild_type=Release -sos=Windows'
                                }
                                else {
                                  sh 'conan install . -sbuild_type=$BuildType --build=missing -sos=Windows'
                                }
                            }                 
                  }
              }

              stage('Configure') 
              {    
                steps 
                {
                  sh 'echo "Configuring..."'        
                  sh 'cmake --preset conan-$buildTypeLower'       
                }
              }

               stage('Build') {    
                  steps {
                    sh 'echo "Building..."'        
                    sh 'cmake --build --parallel 4  --preset conan-$buildTypeLower'        
                  }      
                }

                stage('Test') 
                {
                  steps
                  {
                    sh 'echo "Running Unit Tests..."'   
                    sh 'ctest -V --build-config $BuildType --test-dir build/$BuildType --output-junit  unitTestReports.xml'                  
                  }
                  post {
                    success  {
                        junit (testResults:'build/$BuildType/unitTestReports.xml', allowEmptyResults : true)
                    }
                  }
                }

                stage('Package')
                {
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

                stage("Publish to nexus")
                {
                      steps 
                      {
                          script {
                            if (BuildType == 'Release') {
                              sh 'echo "Publish package to Nexus..."'                      
                              // Find built artifact under target folder
                              filesByGlob = findFiles(glob: "_PACKAGE_/*.sh");                    
                              // Extract the path from the File found
                              artifactPath = filesByGlob[0].path;
                              // Assign to a boolean response verifying If the artifact name exists
                              artifactExists = fileExists artifactPath;                   
                              //Extract current version
                              versionExtracted = artifactPath.split('-')[1]                    
                              if(artifactExists) {
                                  echo "*** File: ${artifactPath}";
                                  nexusArtifactUploader(
                                      nexusVersion: NEXUS_VERSION,
                                      protocol: NEXUS_PROTOCOL,
                                      nexusUrl: NEXUS_URL,
                                      groupId: "org.stroalgo",
                                      version: "Windows-${BuildType}-${versionExtracted}",
                                      repository: NEXUS_REPOSITORY,
                                      credentialsId: NEXUS_CREDENTIAL_ID,
                                      artifacts: [                                
                                          [artifactId: NEXUS_REPOSITORY,
                                          classifier: '',
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

            
            }
      }
  } 
  
    post {
            always {
                
                emailext attachLog: true,
                    body: "${currentBuild.currentResult}: Job ${env.JOB_NAME} build ${env.BUILD_NUMBER}\n More info at: ${env.BUILD_URL}",
                    recipientProviders: [developers(), requestor()],
                    subject: "Jenkins Build ${currentBuild.currentResult}: Job ${env.JOB_NAME}"
                
            }
        } 
  




}