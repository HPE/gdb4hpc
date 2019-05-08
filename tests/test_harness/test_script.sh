#!/bin/bash


create_regression_report(){

    #find the most recent reports branch having same test_list

    rep_dirs=`ls -l $mstr_reports_dir | grep '^d' | awk '{print $9}'`
    timediff=0
    timeneeded=0
    dirname=""

    while IFS=$'\n' read -ra dirs; do

        for p in "${dirs[@]}"; do

           #find latest release 
           if [[ $p == *$bucket*  && $p == *$runnr* ]]
           then 
    		   #get the timestamp
		       timestmp=`echo $p | cut -d'_' -f5`
		       timediff=`echo "$t4-$timestmp" | bc -l`

		       if [[ $timeneeded == 0 ]]
		       then
		           #echo "first it " $timediff
		           timeneeded=$timediff
		       elif [[ $timeneeded -gt $timediff  && $timediff -ne 0 ]]
		       then
		           #echo "next it " $timediff
		           timeneeded=$timediff
		           dirname=$p
		       fi
	      fi
	  done
    done<<<"$rep_dirs"

    index0=0
    index1=0
    echo -e "output_file\tpass" >>  $mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id/regressions.rep
    echo -e "output_file\tpass" >>  $mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id/persistent_failures.rep
    echo -e "output_file\tpass" >>  $mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id/new_failures.rep
    echo -e "output_file\tpass" >>  $mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id/fixed.rep
    
    #get child directory of dirname
    subdir=`ls -l $mstr_reports_dir/$dirname | grep '^d' | awk '{print $9}'`
    
    prev_test="$mstr_reports_dir/$dirname/$subdir/${tblFile}"
    new_test="$mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id/${tblFile}"
    sleep 4
    prev_report="$(<$prev_test)"
    new_report="$(<$new_test)"


    #read in line by line the aesop.rep from this test
    while IFS=$'\n' read -ra line; do

	for p in "${line[@]}"; do

	    if [[ $index0 -gt 0 ]]
	    then

		current_file=`echo $p | cut -f1 -s`
		current_result=`echo $p | cut -f4 -s`
		
		#echo old_file $old_file
		#echo old_result $old_result
		found=false

		 #read in line by line the aesop.rep from the previous test
		 while IFS=$'\n' read -ra old_file_line
		 do
		     for x in "${old_file_line[@]}"; do
    
			 if [[ $index1 -gt 0 ]]
			 then

			     old_file=`echo $x | cut -f1 -s`
			     old_result=`echo $x | cut -f4 -s`
			     #echo current file $current_file
			     #echo current result $current_result
			     

			     if [[ $old_file == $current_file ]]
			     then
				

				 if [[ $old_result == "Y" && $current_result == "N" ]]
				 then
				     
					#have a regression
					echo -e "$current_file\t$current_result" >>$mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id/regressions.rep
					found=true
					break
				 elif [[ $old_result == "N" && $current_result == "N" ]]
				 then
				     
					#have a persisting failure
					echo -e "$current_file\t$current_result" >>$mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id/persistent_failures.rep
					found=true
					break
				 elif [[ $old_result == "N" && $current_result == "Y" ]]
				 then
				     
					#have fixed a failure which occured in the previous version
					echo -e "$current_file\t$current_result" >>$mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id/fixed.rep
					found=true
					break
				 fi
			     fi

			 fi
			 index1=$((index1+1))
		    done
		 done<<<"$prev_report"
		 
		 if [[ $found == false ]] 
		 then
		     #there is no matching file/test in the previous regression test
		     #thus there is a new test in the lgdb test harness
		     if [[ $current_result == "N" ]]
		     then
			 #have a failure in a new test
			 echo -e "$current_file\t$current_result" >>$mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id/new_failures.rep
		     fi
		 fi


	
	    fi
	    index0=$((index0+1))
	done
    done<<<"$new_report"


}




handle_reports(){
    
   mkdir $mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4
   if [ ! -d $mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id ]
   then
     mkdir $mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id
   fi
   #echo *.rep $mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id
   cp *.rep $mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id

   for p in "${arlocs[@]}"; do
      
      #determine if there is both a parent directory and a child directory in the path
      parent_and_child=`echo $p | grep /`
      if [[ $parent_and_child  != "" ]]
      then
        parent_dir=`echo $p | cut -d'/' -f1`
        child_dir=`echo $p | cut -d'/' -f2`
        if [ ! -d  $mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id/$parent_dir ]
        then
          mkdir $mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id/$parent_dir
        fi
        if [ ! -d $mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id/$parent_dir/$child_dir ]
        then
          mkdir $mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id/$parent_dir/$child_dir
        fi
      else
	    parent_dir=$p
        mkdir $mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id/$parent_dir
      fi

      cp $p/*.rep $mstr_reports_dir/$lgdb_version"_"$bucket"_"$runnr"_"$t4/"test_id_"$test_id/$p
      
   done

}


compare_output_linebyline(){

    #loop through the directories and open each test.txt file
    #open associated test.out file
    #get a line from .txt

    #search linebyline for matching string

    #search each string for address regex, if found remove, then compare
    excount=0
    othcount=0
    dt=""
    strGNU="GNU gdb"
    strCopyright="Copyright"
    strAddress="shared struct data_struct"
    LGDB_V="Version"
    cmdState=false

    echo
    echo "Determining results. . ."
    echo


    #for each directory. . .
    for p in "${arlocs[@]}"
    do
      #get into that directory
      nm=`echo $p | sed 's#\./##g'`
      freq=`echo $nm | tr -dc '/' | wc -c`

      cd $nm
      echo `pwd`

      outfiles=`find . -name "*.out"`
      #echo "my outfiles $outfiles: "

      txtfiles=`find . -name "*.txt"`
      #echo "my txtfiles $txtfiles:"

      #if there are files in the subdirectory
      while IFS=$'\n' read -ra outfilelist; do

        #for each file in the master .txt list
        for x in "${outfilelist[@]}"; do

            xpath=`echo "$x" | sed 's/.\(.*\)/\1/'`
            cmpname=`echo "$x" | cut -d'.' -f2`
            cmpname=`echo "$cmpname" | cut -d'/' -f2`
                
            substr2="Breakpoint [0-9]:"

			pe=0


            #check file to determine if launch error/Could not start application
            #if so, nothing to compare, echo to stdout and continue to next file
            appFail=`cat ${x} | grep "Could not startup application."`
            launchFail=`cat ${x} | grep "Aprun launch failed."`
            if [[ $appFail != "" || $launchFail != "" ]]
            then
                echo "FAILED to launch"
                failures[g]="$nm/$cmpname-$pe"
                g=$((g + 1))
                continue
            fi  
            #echo
            bkpts=`cat ${x} | grep "${substr2}" | sed -e 's/.*line //g;s/\.$//g' | sort -u`

            #get the source file name - there could be multiple source files run in lgdb
            #thus must put them into array. When find the string "breakpoint [0-9] in .out
            #must capture each source files to which a breakpoint is associated.
            cmp_src=`cat ${x} | grep "${substr2}" | sed -e 's/,.*//g;s/\.$//g;s/.*\///g'` #| head -n1`
                        
            i=1
            #loop through bkpts and put into array
            while IFS=$' ' read -ra bkptlist; do
                for bkpt in "${bkptlist[@]}"; do
                    #get the line number
                    arBkpt[i]="$bkpt"
                    i=$((i + 1))
                done
            done<<<"$bkpts"

            fndPass=""
            errState=""
            vers=0.0
            oldvers=0.0
		    foundmatchingoutfile=false

            #read master .txt file
            while read outfileline; do
				    
                #search for string-> letter{number}: and remove this portion
				#from the compare string. Because user may test varying number
				#of ranks.
			      
				outfileline=`echo $outfileline | sed 's/[a-z]{[0-9]\.\.[0-9]}//g'` 

                #for each output file
                while IFS=$'\n' read -ra textfilelist; do

                    for y in "${textfilelist[@]}"; do
			
                        #echo "source file name $y"
                        outname=`echo "$y" | cut -d'.' -f2`
                        outname=`echo "$outname" | cut -d'/' -f2`

						#if that output file name matches the master .txt file name, compare
                        if [[ $outname == $cmpname ]]
						then
							if [[ $foundmatchingoutfile == false ]]
							then
								foundmatchingoutfile=true
							    echo "Determining results for $cmpname"
								pe=`cat ${cmpname}.out | grep -m1 "Initial breakpoint" | sed 's/[a-z]{[0-9]\.\.//1g' | awk -F} '{print $1}'`
			                    if [[ "$pe" == "" ]]
			                    then
				                    pe=`cat ${cmpname}.out | grep -m1 "Debugger error" | sed 's/[a-z]{[0-9]\.\.//1g' | awk -F} '{print $1}'`
			                    fi

			                    if [[ $pe == "" ]]
			                    then
				                  pe=0
			                    else
				                  pe=$((pe + 1))
			                    fi
							fi
							        
                            fndPass="PASSED $outfileline"
                            cmdState=false
								
							#for each line in the output file
                            while read txtfileline; do

							    txtfileline=`echo $txtfileline | sed 's/[a-z]{[0-9]\.\.[0-9]}//g'`

                                #if the source file path is in the line
                                while IFS=$' ' read -ra srcfs; do
                                    for sourcefile in "${srcfs[@]}"; do
										srcname=`echo $sourcefile | cut -d'.' -f1`
										      
                                        if [[ $outfileline == *$srcname* && $txtfileline == *$srcname* ]]
                                        then
											if [[ $outfileline == "*reakpoint*" && $txtfileline == "*reakpoint*" ]]
											then    
                                                lineno2=0
                                                #confirm we are at a legit breakpoint
                                                lineno2=`echo $txtfileline | sed -e 's/.*://g;s/.*line //g;s/\.$//g'`

                                                #confirm the breakpoint is correct
                                                for bkpt in "${arBkpt[@]}"; do

                                                  #echo "array lineno" $bkpt
                                                  if [[ $bkpt == $lineno2 ]]
                                                  then
                                                      cmdState=true
                                                      `echo $fndPass >> $outname.rep`
                                                      break 3
                                                  fi
                                                done
    
                                            #if sourcefile is testing the info command, cannot get breakpoint from it, must
										    #ignore because n_pes, nid, date in path will all vary from test to test.
											elif [[ $outfileline != "*reakpoint*" && $txtfileline != "*reakpoint*" ]]
											then
												cmdState=true
												`echo $fndPass >> $outname.rep`
												break 2
											fi
                                        fi
                                    done
                                done<<<"$cmp_src"

								#ignore dbgsrvs connected line
								if [[ $outfileline == *"dbgsrvs connected"* && $txtfileline == *"dbgsrvs connected"* ]]
								then

								    cmdState=true
									`echo $fndPadd >> $outname.rep`
									break

								#ignore test output related to rank indexes
								elif [[ $outfileline == *"rank"* && $txtfileline == *"rank"* ]]
								then
									cmdState=true
									`echo $fndPass >> $outname.rep`
									break

								#ignore test output related to process
								elif [[ $outfileline == *"from process"* && $txtfileline == *"from process"* ]]
								then
									cmdState=true
									`echo $fndPass >> $outname.rep`
									break

                                #ignore initial breakpoint location
                                elif [[ $outfileline == *"Initial breakpoint"* && $txtfileline == *"Initial breakpoint"* ]]
                                then
                                    cmdState=true
                                    `echo $fndPass >> $outname.rep`
                                    break

                                #ignore gnu version
                                elif [[ $outfileline == *$strGNU* &&  $txtfileline == *$strGNU* ]]
                                then
                                    cmdState=true
                                    vers=`echo "$outfileline" | awk '{print $5}' | sed -e 's/-.*//'`
                                    oldvers=`echo "$txtfileline" | awk '{print $5}' | sed -e 's/-.*//'`

                                    if [[ $vers != $oldvers ]]
                                    then
                                        echo "Warning: current GDB version: " $vers", old version: " $oldvers
                                    fi
                                    `echo $fndPass >> $outname.rep`
                                    break

                                #ignore copyright year
                                elif [[ $outfileline == *$strCopyright* && $txtfileline == *$strCopyright* ]]
                                then
                                    cmdState=true
                                    #yr=`echo "$outfileline" |  grep -o '[0-9]\{4\}'`
                                    #oldyr=`echo "$txtfileline" |  grep -o '[0-9]\{4\}'`
                                    #if [[ $yr != $oldyr ]]
                                    #then
                                        #echo "Warning: Current copyright date: " $yr" , old copyright date: " $oldyr
                                    #fi
                                    `echo $fndPass >> $outname.rep`
                                    break
                                
                                #ignore lgdb version
                                elif [[ $outfileline == $LGDB_V* ]]
                                then 
                                    cmdState=true
                                    `echo $fndPass >> $outname.rep`
                                    break

                                #ignore different shared struct address
                                elif [[ $outfileline == *$strAddress* && $txtfileline == *$strAddress* ]]
                                then
                                    shd_struct_addy=`echo $outfileline | awk '{print $6}' | sed 's/,.*//g;s/.*x//g'`
                                    if [[ $shd_struct_addy != "" ]]
                                    then
                                        cmdState=true
                                        `echo $fndPass >> $outname.rep`
                                        break
                                    fi

                                #lines match exactly
                                elif [[ "$outfileline" == "$txtfileline" ]]
                                then
                                    cmdState=true
                                    `echo $fndPass >> $outname.rep`
                                    break

                                #need to disregard timing. just ensure captured "start", "finish", and "duration" text.
                                elif [[ $outfileline == *start* && $txtfileline == *start* ]]
                                then
                                    cmdState=true
                                    `echo $fndPass >> $outname.rep`
                                    break

                                elif [[ $outfileline == *finish* && $txtfileline == *finish* ]]
                                then
                                    cmdState=true
                                    `echo $fndPass >> $outname.rep`
                                    break

                                elif [[ $outfileline == *duration* && $txtfileline == *duration* ]]
                                then
                                    cmdState=true
                                    `echo $fndPass >> $outname.rep`
                                    break

                                #ignore assertion locations as they will be determined dynamically
        						elif [[ $outfileline == *"Application halted in"* && $txtfileline == *"Application halted in"* ]]
								then
									strFound=""
                                    assertFound=""
									strFound=`cat ${cmpname}.out | grep "Current location:"`
									assertFound=`cat ${cmpname}.out | grep "assert"`
									if [[ $assertFound != "" && $strFound != "" ]]
									then
										cmdState=true
										`echo $fndPass >> $outname.rep`
										break
									fi
                                fi                                                                           
                                                                        
                            done<$y

                            if [[ $cmdState == false ]]
                            then
                                #there was an error on this line
                                fndError="ERROR found on line $outfileline"
                                echo "ERROR found on line $outfileline"
                                errState="FAILED"

                                #append to testname.rep Error
                                `echo $fndError >> $outname.rep`
                                testErrorCount=$((testErrorCount+1))

                            elif [[ $cmdState == true ]]
                            then
								#it appears the test ran fine.
                                #make sure the test didn't error in another way.
                                #check test_status array. if inside then it did fail
                                #in some way

                                #for each file in the test_status array
                                for fl in "${test_status[@]}"; do

                                    if [[ $fl == $outname.out ]]
                                    then
					                    echo run error
                                        errState="FAILED"
										testErrorCount=$((testErrorCount+1))
                                        break
									fi
                                done
                            fi
                        fi


                    done
				done<<<"$outfiles" #was txtfiles
					
            done<$x


			if [[ $testErrorCount -gt 0 ]]
			then
				failures[g]="$nm/$cmpname-$pe"
                g=$((g + 1))
			else
				passed[h]="$nm/$cmpname-$pe"
				h=$((h+1))
			fi

			if [[ $foundmatchingoutfile == true ]]
			then
				echo $newline >> $tresults
				echo "----------------------------------------------------" >> $tresults
				echo "--------------- RESULTS FOR $cmpname ----------------" >> $tresults
				echo "----------------------------------------------------" >> $tresults
				echo >> $tresults

				echo "Overall Test results: " >> $tresults
				if [[ $errState == "" ]]
				then
					cat ${cmpname}.rep >> $tresults
					echo $newline >> $tresults
					echo "PASSED" 2>&1 | tee -a $tresults
				else
					cat ${cmpname}.out >> $tresults
					echo $newline >> $tresults
					echo "FAILED" 2>&1 | tee -a $tresults
				fi
			fi
        done
		testErrorCount=0

        done<<<"$txtfiles" #was outfiles

        #if two // in path, make sure go back cd ../..
        #else go back cd ..
        if [[ $freq == 2 ]]
        then
            cd ../../..
        elif [[ $freq == 1 ]]
        then
            cd ../..
        else
            cd ..
        fi

    done

	echo 2>&1 | tee -a $tresults
	echo 2>&1 | tee -a $tresults
    echo "--------------------------------------" 2>&1 | tee -a $tresults
    echo "             Test Summary             " 2>&1 | tee -a $tresults
    echo "--------------------------------------" 2>&1 | tee -a $tresults
	echo 2>&1 | tee -a $tresults
	echo "Test list: " $bucket

    echo 2>&1 | tee -a $tresults

    #Send system info to tblFile
    echo -e "GDB4HPC Nigthly Functional Testing:\t$lgdb_version" >> $tblFile
    echo -e " " >> $tblFile
    echo -e "HOST:\t$MACHINE" >> $tblFile
    echo -e "Test Type:\tCPU Sanity Test Suite" >> $tblFile
    echo -e "Bucket:\t$bucket" >> $tblFile
    echo -e "Workload Manager:\t$wlm_ver" >> $tblFile
    echo -e "Architecture:\t$arch" >> $tblFile
    echo -e "Operating System:\t$os" >> $tblFile
    echo -e "CLE:\t$cle\tcompiler" >> $tblFile
    echo -e "Targeted CPU:\t$cpu\t$cce" >> $tblFile
    

	#printf "%-40s%-20s%-20s%-5s\n" "output_file" "npes" "pass" >> $tblFile
    echo -e "output_file\tnpes\tpass" >> $tblFile
        
    if [[ ${#failures[@]} -gt 0 ]]
    then
        echo "The following tests FAILED" 2>&1 | tee -a $tresults
    	printf "%-40s%-20s\n" "TESTNAME" "PES" 2>&1 | tee -a $tresults

        #capture unique filenames only
        arFail=($(printf "%s\n" "${failures[@]}" | sort -u))
        for fls in "${arFail[@]}"; do

		    tstpes=`echo ${fls} | cut -d'-' -f2`
		    tstNm=`echo ${fls} | cut -d'-' -f1`
			   
		    printf "%-40s%-20s\n" $tstNm $tstpes 2>&1 | tee -a $tresults

		    #check if this is test results from a source file or tests for SIG
		    tst_type=`echo $tstNm | grep "SIG"`
		    if [[ $tst_type == "" ]]
		    then
			   echo -e "$tstNm\t$tstpes\tN" >> $tblFile
		    fi
		       
        done
    fi


    echo >> $tresults
    if [[ ${#passed[@]} -gt 0 ]]
    then
        echo "The following tests PASSED." 2>&1 | tee -a $tresults
		printf "%-40s%-20s\n" "TESTNAME" "PES" 2>&1 | tee -a $tresults
		

        #capture unique filenames only
        arPass=($(printf "%s\n" "${passed[@]}" | sort -u))
        for fls in "${arPass[@]}"; do

		    tstpes=`echo ${fls} | cut -d'-' -f2`
		    tstNm=`echo ${fls} | cut -d'-' -f1`
		       
		    printf "%-40s%-20s\n" $tstNm $tstpes 2>&1 | tee -a $tresults

		    #check if this is test results from a source file or tests for SIG
		    tst_type=`echo $tstNm | grep "SIG"`
		    if [[ $tst_type == "" ]]
		    then
			   echo -e "$tstNm\t$tstpes\tY" >> $tblFile
		    fi
        done
        
	fi
}

check_env(){
        
        volta=$(module list -l 2>&1 > /dev/null | grep 'craype-accel-nvidia270')
        if [[ "$volta" != "" ]]
        then
                echo 2>&1 | tee -a $blog
                echo "The craype-accel-nvidia70 module was loaded." 2>&1 | tee -a $blog
        fi
        pascal=$(module list -l 2>&1 > /dev/null | grep 'craype-accel-nvidia60')
        if [[ "$pascal" != "" ]]
        then
                echo 2>&1 | tee -a $blog
                echo "The craype-accel-nvidia60 module was loaded." 2>&1 | tee -a $blog
        fi

        kepler=$(module list -l 2>&1 > /dev/null | grep 'craype-accel-nvidia35')
        if [[ "$kepler" != "" ]]
        then
                echo 2>&1 | tee -a $blog
                echo "The craype-accel-nvidia35 module was loaded." 2>&1 | tee -a $blog
        fi

        fermi=$(module list -l 2>&1 > /dev/null | grep 'craype-accel-nvidia20')
        if [[ "$fermi" != "" ]]
        then
                echo 2>&1 | tee -a $blog
                echo "The craype-accel-nvidia20 module was loaded." 2>&1 | tee -a $blog
        fi

}
update_rcfile(){

        #update the rc file with the npes

        #$1: name of rc file
        #$2: npes
        ss="launch "
        str="launcher"
        ranks=`echo $npes/$exec_count | bc`

        #need to get the procset from each launch command
        launch_cmds=`cat ${location}/${rc_file} | grep ${str}`

        idCount=0
        while IFS=$'\n' read -ra launch; do

	     for y in "${launch[@]}"; do
		    #get the letter from the procset designation
		    arProcSet[idCount]=`echo $y | cut -d' ' -f2 | sed 's/{[0-9]*}//g' | sed 's/\$//g'`
		    idCount=$((idCount + 1))
	     done
        done<<<"$launch_cmds"

        for procSet in "${arProcSet[@]}"; do
            sed -i "s|$ss$procSet{[1-9][0-9]*|$ss$procSet{$ranks|g" $location/$rc_file
            #cat $location/$rc_file | grep launch
		
		    #if this runnr = slurm, then replace '-N1' with --exclusive into lgdb launch command
		    if [[ $wlm = "SLURM" ]]
		    then
                #sed -i "s|--exclusive||g" $location/$rc_file
		        sed -i "s|-N1|--exclusive|g" $location/$rc_file
	        elif [[ $wlm = "ALPS" ]]
            then
		        sed -i "s|--exclusive|-N1|g" $location/$rc_file               
            fi
        done
        
        #if the test has decomposition in it, update the decomposition with
        #correct ranks
        decomp=`cat ${location}/${rc_file} | grep dimension`
	    proc_grid=`cat ${location}/${rc_file} | grep proc_grid`
	    if [[ $decomp != "" && $proc_grid != "" ]]
	    then
	        sed -i "s|dimension [0-9]*|dimension ${ranks}|g" $location/$rc_file
	        sed -i "s|proc_grid [0-9]*|proc_grid ${ranks}|g" $location/$rc_file
	    fi
        
        #update the batch script
        if [[ $batch_mode != "" ]]
        then
            update_batchScript
            result=`cat ${location}/${rc_file} | grep session`
            if [[ $result == "" ]] 
            then
              #inject the session_command in front of launch command in the RC file
              sed -i "s|maint set sync on|session ${session_command}\nmaint sleep 15\nmaint set sync on\n|" $location/$rc_file
            else
              sed -i "/^session/d" $location/$rc_file
              sed -i "/^maint sleep 15\n/d" $location/$rc_file
              sed -i "s|maint set sync on|session ${session_command}\nmaint sleep 15\nmaint set sync on\n|" $location/$rc_file
            fi
        else
          #Make sure session command is not present in .rc file
          sed -i "/^session/d" $location/$rc_file
          sed -i "/^maint sleep 15\n/d" $location/$rc_file    
        fi

}

update_batchScript(){

  #find batch script template
  if [[ $wlm == "ALPS" ]]
  then
    batch_template="qsub.template"
    session_command="--qsub="
  elif [[ $wlm == "SLURM" ]]
  then
    batch_template="sbatch.template"
    session_command="--sbatch="
  fi

  #cp as batch.script
  cp $batchScript_location/$batch_template $batchScript_location/batch.script 
  session_command=${session_command}${batchScript_location}/batch.script

  #update resource info on template
  sed -i "s|pes|${npes}|g" $batchScript_location/batch.script
  sed -i "s|ppn|${ranks}|g" $batchScript_location/batch.script

}




compileAndLink(){

echo
while IFS=$'\n' read -ra line
do

    if [[ $index -gt 0 ]]
    then

        echo $newline | tee -a $blog

        location=`echo $line | awk -F\| '{print $1}' | sed 's/[ \t]*$//g'`
        source_files=`echo $line | awk -F\| '{print $2}' | sed 's/[ \t]*$//'`
        options=`echo $line | awk -F\| '{print $3}' | sed 's/[ \t]*$//'`
        exec_files=`echo $line | awk -F\| '{print $4}' | sed 's/[ \t]*$//'`
        link=`echo $line | awk -F\| '{print $5}' |  sed 's/\r$//'`

        echo 2>&1 | tee -a $blog
        IFS=" "
        lpCount=0
        objfiles=""
        name=""
        echo "$source_files" 2>&1 | tee -a $blog
        for srcfile in $source_files
        do
            if [ ! -r $location/$srcfile ]; then
                echo
                echo "WARNING: File $srcfile is not accessible" 2>&1 | tee -a $blog
                echo "Skipping test for $exec_files"  2>&1 | tee -a $blog
                compileErrorCount=$((compileErrorCount+1))
                continue 2
            fi

            innerLp=0
            for element in $exec_files
            do
                if [[ $lpCount == $innerLp ]]
                then
                    exec=${element}
                    break
                fi
                innerLp=$((innerLp+1))
            done

            if [ "${srcfile%.*}" = '*' ]; then
                continue #no source file found
            fi

            #get basename
            name=${srcfile##*/}
            objfiles="${objfiles} $location/${name%.*}.o"

            #echo 2>&1 | tee -a $blog
            echo "Compiling. . . " 2>&1 | tee -a $blog

            case ${srcfile} in
                *.c)
                        if [[ $link == "Y" ]]
                        then
                                echo "  cc $options -I $include -c $location/$srcfile -o $location/${name%.*}.o" 2>&1 | tee -a $blog
                                RESP=`cc $options  -I $include -c $location/$srcfile -o $location/${name%.*}.o`
                        else
                                echo "  cc $options  $location/$srcfile -o $location/$exec" 2>&1 | tee -a $blog
                                RESP=`cc $options $location/$srcfile -o $location/$exec`
                        fi
                    ;;
                *.cc|*.cpp|*.cxx)
                        if [[ $link == "Y" ]]
                        then
                        echo "  CC $options -I $include -c $location/$srcfile -o $location/${name%.*}.o" 2>&1 | tee -a $blog
                        CC $options  -I $include -c $location/$srcfile -o $location/${name%.*}.o
                    else
                                                echo "  CC $options $location/$srcfile -o $location/$exec" 2>&1 | tee -a $blog
                                CC $options $location/$srcfile -o $location/$exec
                    fi
                   ;;
               *.[fF])
                        if [[ $link == "Y" ]]
                        then
                        echo "  ftn $options -I $include -c $location/$srcfile -o $location/${name%.*}.o" 2>&1 | tee -a $blog
                                ftn $options  -I $include -c $location/$srcfile -o $location/${name%.*}.o
                    else
                                                echo "  ftn $options $location/$srcfile -o $location/$exec" 2>&1 | tee -a $blog
                                ftn $options $location/$srcfile -o $location/$exec
                    fi
                    ;;
                *.[fF]90)
                        if [[ $link == "Y" ]]
                                then
                                echo "  ftn $options -I $include -c $location/$srcfile -o $location/${name%.*}.o" 2>&1 | tee -a $blog
                                RESP=`ftn $options  -I $include -c $location/$srcfile -o $location/${name%.*}.o 2>&1`
                        else
                                                echo "  ftn $options $location/$srcfile -o $location/$exec" 2>&1 | tee -a $blog
                                RESP=`ftn $options $location/$srcfile -o $location/$exec 2>&1`
                        fi
                                        ;;
            esac
            lpCount=$((lpCount+1))
            echo
            #warning=`echo $RESP | grep ptxas`
            if [[ "$RESP" != "" ]]
            then
                echo "There was an error compiling " $srcfile 2>&1 | tee -a $blog
                echo $RESP | tee -a $blog
                echo "Test will be skipped" 2>&1 | tee -a $blog
                continue
            else
                echo "  $srcfile was successfully compiled." 2>&1 | tee -a $blog
            fi
            echo
        done


        if [[ $link == "Y" ]]
        then
            case $srcfile in
            *.c)
                LINKER=cc ;;
            *.cc|*.cpp|*.cxx)
                LINKER=CC ;;
            *.upc)
                LINKER=cc ;;
            *.f|*.F)
                LINKER=ftn ;;
            *.f90|*.F90)
                LINKER=ftn ;;
            esac

            #echo "PTHCOMMAND $PTH_CC file.c" | tee -a [logfile] $PTH_CC file.c |& tee -a $blog

            echo "Linking. . ." 2>&1 | tee -a $blog
            echo "  $LINKER -o $location/$exec $objfiles" 2>&1 | tee -a $blog
            $LINKER -o $location/$exec $objfiles
            failed=`echo  $?`
            if [ "$failed" != "0" ]; then
                echo "Failed linking source files for $source_files . . . skipping test" 2>&1 | tee -a $blog
                continue
            fi

        fi

    fi
    index=$((index+1))

done<<<"$build_src"


}
check_bucket_type() {

if [[ $bucket = "gpu_list" ]]
then
    #is an nvidia accelerator driver loaded?
    check_env
    
    build_src=$(<./gpu_build_src)

    if [[ $kepler == "" && $fermi == "" && $pascal == "" && $volta == "" ]]
    then
        echo "A craype-accel-nvidia module must be loaded for gpu tests." 2>&1 | tee -a $blog
        echo "Exiting. . ." 2>&1 | tee -a $blog
        exit 1
    fi
else
    build_src=$(<./build_src)
fi

}

get_system_info() {

  arch=`uname -m`
  if [ -f /etc/SuSE-release ]
  then
    os=$(cat /etc/SuSE-release | grep VERSION | cut -f3 -d" ")
  elif [ -f /etc/redhat-release ]
  then
    cs_ver=`cat /etc/redhat-release | cut -d' ' -f7`
    os="CS $cs_ver"
  elif [ -f /etc/os-release ]
  then
    os=$(cat /etc/os-release | grep 'VERSION_ID="15"' | cut -d'"' -f2)
  fi
  cpu=` echo $CRAY_CPU_TARGET`
  if [ -z $cpu ]
  then 
    cpu="Not loaded"
  fi
  if [ -f /etc/opt/cray/release/cle-release ]
  then
    cle=`cat /etc/opt/cray/release/cle-release | awk -F\= '{print $2}' | head -1`
    cle_date=`cat /etc/opt/cray/release/cle-release | awk -F\= '{print $2}' | head -3 | tail -1`
    cle=$cle-$cle_date
  else
    cle=""
  fi
  cce=`echo $CRAY_PRGENVCRAY`
  if [ $cce == 'loaded' ]
  then
    cce_ver=`echo $CRAY_CC_VERSION`
    cce="cce/$cce_ver"
  else
    cce='Not loaded'
  fi

}


newline="      "
node_type=""
DIFF=0
TOTAL=0
end=
start=
index=0
testname=""
test_type=""
source_command=""
Timeout=40
username=""
lgdb_command="gdb4hpc"
src_command="-s"
batch_command="-b"
session_command=""
batch_file=""
batchScript_location=$PWD
batch_mode=$3
include=include
ind=0
h=0
g=0
arloc=""
kepler=""
fermi=""
pascal=""
volta=""
NETWORK=$CRAYPE_NETWORK_TARGET
MACHINE=$HOST
test_status=""
failed_int=0
RESP=""
exec_count=0
compileErrorCount=0
testErrorCount=0
wlm=""
wlm_ver=""
lgdb_version=""
runnr=""
user=`echo $USER`
mstr_reports_dir=/cray/css/ulib/gdb4hpc/regression_testing
decomp=""
proc_grid=""
bucket=$1
compile_state=0
npes=6
t1=""
t2=""
t3=""
t4=""
TEST_VER=v2.0
test_dir=test_harness
tblFile="aesop.rep"
blog="build_log.rep"
tlog="test_log.rep"
tresults="results_log.rep"
test_id=`date '+%Y-%m-%d-%H%M%S'`
who=`whoami`
arch=""
os=""
cle=""
cle_date=""
cpu=""
cce=""
cce_ver=""


if [[ $2 == "build" ]]
then
    compile_state=1
elif [[ $2 == "run" ]]
then
    compile_state=2
fi

#name of bucket
tlist=$(<./$bucket) #NAME OF THE TEST SUITE SELECTED IN AESOP


#compile all of the source files for selected listfile
if [[ $compile_state -eq 1 ]]
then
        t1=`date +"%s"`
        #cleanup the dev tree
        source cleanup $bucket
        sleep 2

        check_bucket_type
        compileAndLink

        t2=`date +"%s"`
        ttot=`echo "$t2-$t1" | bc -l`


        #determine build status
        if [[ "$RESP" == "" && $compileErrorCount -eq 0 ]]
        then
            b_status="C"
            b_status_desc="Completed Successfully."
        elif [[ "$RESP" != ""  || $compileErrorCount -gt 0 ]]
        then
            b_status="E"
            b_status_desc="Failed"
        fi
        echo 2>&1 | tee -a $blog
        echo Build status: $b_status : $b_status_desc 2>&1 | tee -a $blog
        $eini $ini set build_status ${b_status}

elif [[ $compile_state -eq 2 ]]
then
        module load wlm_detect
        wlm=`wlm_detect`

        t3=`date +"%s"`

        if  [[ $wlm == "ALPS" ]]
        then
          wlm_ver=`echo $CRAY_ALPS_INCLUDE_OPTS | cut -d'/' -f5`
	  runnr="aprun"
        elif [[ $wlm == "SLURM" ]]
        then
          wlm_ver=`srun --version`
	  runnr="srun"
        fi

	    check_bucket_type

        echo
        ldMod=$(module list -l 2>&1 > /dev/null | grep 'gdb4hpc' )
        if [[ "$ldMod" == "" ]]
        then
          echo "gdb4hpc is not loaded. Exiting. . ." 2>&1 | tee -a $tlog
          exit 1
        else
          echo "Loaded gdb4hpc:" 2>&1 | tee -a $tlog
          echo "  GDB4HPC version: " $ldMod 2>&1 | tee -a $tlog
          lgdb_version=`echo $ldMod | cut -d' ' -f1 | cut -d'/' -f2`
        fi

        echo
        echo "  GDB4HPC Test Harness ver: " $TEST_VER 2>&1 | tee -a $tlog
        echo
        echo "Setting GDB4HPC environment variables in the current shell.. . ." 2>&1 | tee -a $tlog
        export GDB4HPC_LOG_DIR=/lus/scratch/$who
        echo "GDB4HPC log files are located in " $DBG4HPC_LOG_DIR 2>&1 | tee -a $tlog


        # ENV CONFIGURATIONS
        # Ensure the module file is sourced in properly.*********************************************
        if [ -e /etc/modules ] ; then
           source /etc/modules/bash
        fi
        if [ -d /sw/modules/init ] ; then
           source /sw/modules/init/bash
        fi
        if [ -d /opt/modules/default/init ] ; then
           source /opt/modules/default/init/bash
        fi
    
        get_system_info
        
        echo
        echo "TESTING. . . " 2>&1 | tee -a $tlog
        index=0
        echo
        while IFS=$'\n' read -ra line
        do

            if [[ $index -gt 0 ]]
            then

                testname=`echo $line | awk -F\| '{print $1}' | sed -e 's/[ \t]*//g'`
		        location=`echo $line | awk -F\| '{print $2}' | sed 's/[ \t]*$//'`
                source_command=`echo $line | awk -F\| '{print $3}' | sed 's/[ \t]*$//'`
                rc_file=`echo $source_command | awk '{print $1}' `
                outfile=`echo $source_command | awk '{print $2}'`

                #get number of executables run in each test
                exec_count=0
			    exec_count=`cat ${location}/${rc_file} | grep launcher | wc -l`

                oName=`echo $outfile | tr "=" "\n"`
                IFS=$'\n'
                ptcount=0
                for pt in $oName
                do
                    if [[ $ptcount -eq 1 ]]
                    then
                        oName=$pt
                        break
                    fi
                    ptcount=$((ptcount+1))
                done

                outfile=`echo $outfile | sed "s|--output=|--output="$location/"|g"`

                #update rc file with npes
                echo "updating rc file. . ."
		        update_rcfile


                #put location into array for later use in compare function
                arloc[$ind]="${location}"
                ind=$((ind+1))


                echo
                IFS=" "
                lpCount=0
                objfiles=""
                name=""

                IFS='%' #need to ensure initCLI doesnt throw error
                START=$(date +%s)

                echo
                echo "Running $testname . . ." 2>&1 | tee -a $tlog

                echo "  $lgdb_command $src_command $batch_command $location/$rc_file $outfile" 2>&1 | tee -a $tlog
                $lgdb_command $src_command $batch_command $location/$rc_file $outfile &

    		pid=$!
			#echo lgdb pid is: $pid 2>&1 | tee -a $tlog
			#create timer for lgdb, if times out, force lgdb exit, set aesop error state
			EXIT=0
			lgdb_counter=0

			while [[ $lgdb_counter -lt $Timeout ]]; do
			    lgdb_counter=$((lgdb_counter + 1))
			    sleep 1

			    #if lgdb has completed, then exit do loop
			    kill -0 $pid 2> /dev/null
			    if [[ $? == 1 ]]
			    then
				    echo "  Application $testname successfully ran to completion." 2>&1 | tee -a $tlog
				    EXIT=1
				    break
			    fi
			done


    		if [[ $EXIT == 0 ]] 
			then
			    echo 2>&1 | tee -a $tlog
			    echo "  LGDB timed out or is hanging. Killing gdb4hpc session." 2>&1 | tee -a $tlog
			    echo "  Skipping test for $testname"  2>&1 | tee -a $tlog

			    kill -9 $pid 2>&1 /dev/null
			    if [[ $? == 0 ]]
			    then
				echo "  This LGDB instance (pid: $pid) was successfully killed." 2>&1 | tee -a $tlog
			    else
				echo "  This LGDB instance (pid: $pid) was not successfully killed." 2>&1 | tee -a $tlog
			    fi
			    
			    #is the application still running? (When lgdb is killed, the application should also be killed
			    #ensure that it was. If not, then kill the application.
			    #for each executable in the .rc source file, ensure the application was killed
			    exec_files=`cat $location/$rc_file | grep launcher | cut -d' ' -f5`
			    for ex_file in $exec_files 
			    do
				    #get the executable from path
				    filename=`echo $ex_file | sed 's/.*\///'`

				    apid=""
				    if [[ $wlm = "ALPS" ]]
				    then
					    apid=`apstat | grep $user | grep $filename | cut -d' ' -f1`
					    apkill -9 $apid 2>&1 /dev/null
				    elif [[ $wlm == "SLURM" ]]
				    then
					    apid=`qstat | grep $user | grep $filename | cut -d' ' -f1`
					    qdel $apid 2>&1 /dev/null
				    fi

				    if [[ $apid != "" ]]
				    then

					    if [[ $? == 0 ]]
					    then
						    echo "  The application (apid: $apid) was successfully killed." 2>&1 | tee -a $tlog
					    else
						    echo "  The application (apid: $apid) was not successfully killed." 2>&1 | tee -a $tlog
					    fi
				    fi
			    done

			    echo 2>&1 | tee -a $tlog
			    test_status[failed_int]=$oName
			    failed_int=$((failed_int + 1))
			   
			fi

            END=$(date +%s)
            DIFF=$(( $END - $START ))
            TOTAL=$(( TOTAL + $DIFF ))
            echo
            echo "---------------------------" 2>&1 | tee -a $tlog
            echo "$testname took $DIFF seconds" 2>&1 | tee -a $tlog
            echo "---------------------------" 2>&1 | tee -a $tlog
            echo

            fi

            index=$((index + 1))


        done<<<"$tlist"

        echo
        echo "--------------------------------------" 2>&1 | tee -a $tlog
        echo "Total Test time was $((TOTAL/60)) min $((TOTAL%60)) seconds" 2>&1 | tee -a $tlog
        echo "--------------------------------------" 2>&1 | tee -a $tlog
        echo


        IFS=$'\n'
        #determine unique elements in the array
        arlocs=($(printf "%s\n" "${arloc[@]}" | sort -u))

	    source sig_test ${tlog}

        #run the compare function
        compare_output_linebyline
        
        t4=`date +"%s"`
        ttot=`echo "$t4-$t3" | bc -l`
    
	    #send reports to accessible location    
	    handle_reports
	    
	    #determine regressions
	    create_regression_report

        echo 2>&1 | tee -a $tlog
        echo Test status: $t_status : $t_status_desc 2>&1 | tee -a $tlog
	
fi

unset IFS
exit 0
