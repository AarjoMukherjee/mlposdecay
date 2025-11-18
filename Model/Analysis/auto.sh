#!/bin/bash

# Loop from 0 to 950 in steps of 50
for eLow in $(seq 0 50 950); do
    # Compute eHigh = eLow + 50
    eHigh=$((eLow + 50))

    echo "--------------------------------------------------"
    echo "Running ROOT macro for eLow = $eLow   eHigh = $eHigh"
    echo "--------------------------------------------------"

    # Run ROOT script with parameters and save output to log
    # Adjust 'Plot.C' and parameter names if needed
    root -l -b -q "Plot.C($eLow, $eHigh)" \
        &> "log_${eLow}_${eHigh}.txt"

    echo "ROOT finished for eLow=$eLow. Log saved to log_${eLow}_${eHigh}.txt"

    # After ROOT finishes, run hadd to merge output ROOT files
    outputFile="${eLow}_${eHigh}.root"
    echo "Running hadd -> ${outputFile}"

    hadd -f "${outputFile}" c*.root

    echo "hadd finished -> ${outputFile}"

    # Optional: clean up temporary c*.root (comment if you want to keep them)
    # rm c*.root

    echo ""
done

echo "ALL JOBS COMPLETED."
