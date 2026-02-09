#!/bin/bash

# --- CONFIGURATION ---
PHILO_EXEC="./philo"
LOG_FILE="test_output.log"

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# --- INPUT PARSING ---
REPEAT_COUNT=$1
SERVINGS=$2
DURATION=0

if [ -z "$1" ]; then
    echo -e "${RED}Error: Please provide the number of repetitions.${NC}"
    echo "Usage: ./test_philo.sh <repeat_count> [servings]"
    exit 1
fi

if [ ! -f "$PHILO_EXEC" ]; then
    echo -e "${RED}Error: Executable $PHILO_EXEC not found.${NC}"
    exit 1
fi

# If no servings provided, ask for duration (Infinite Mode)
if [ -z "$SERVINGS" ]; then
    echo -e "${BLUE}No servings parameter detected (Infinite Mode).${NC}"
    read -p "Enter duration to run each test (in seconds): " DURATION
else
    echo -e "${BLUE}Servings parameter detected: $SERVINGS${NC}"
fi

# --- TEST ARRAYS ---

# Format: " ./philo arg1 arg2 arg3 arg4 "
# Note: Do not add servings here, script appends it automatically

TESTS_SHOULD_LIVE=(
    "5 610 200 100"
    "5 800 200 200"
    "5 610 200 200"
    "200 130 60 60"
    "5 600 150 150"
    "199 610 200 200"
    "131 610 200 100"
    "3 200 65 65"
    "198 800 200 200"
    "200 130 60 60"
)

TESTS_SHOULD_DIE=(
    "2 100 60 60"
    "2 100 100 100"
    "2 800 700 110"
    "3 61 60 60"
    "4 190 100 100"
    "5 90 60 60"
    "10 199 100 100"
	"31 599 200 200"
)

# --- HELPER FUNCTIONS ---

run_test() {
    local args="$1"
    local type="$2" # "LIVE" or "DIE"
    local mode="$3" # "SERVINGS" or "INFINITE"
    local passed=0
    
    # Extract number of philosophers (1st arg)
    local num_philo=$(echo $args | awk '{print $1}')
    
    # Construct Command
    local cmd_args="$args"
    if [ "$mode" == "SERVINGS" ]; then
        cmd_args="$args $SERVINGS"
    fi

    echo -e "${YELLOW}Testing: $PHILO_EXEC $cmd_args${NC}"
    
    for ((i=1; i<=REPEAT_COUNT; i++)); do
        echo -n "   Run $i/$REPEAT_COUNT: "
        
        # Execute based on mode
        if [ "$mode" == "SERVINGS" ]; then
            # Run directly, capture output
            $PHILO_EXEC $cmd_args > "$LOG_FILE" 2>&1
        else
            # Run with timeout for infinite mode
            timeout "$DURATION" $PHILO_EXEC $cmd_args > "$LOG_FILE" 2>&1
        fi
        
        # Analyze Output
        local died_count=$(grep -c "died" "$LOG_FILE")
        
        if [ "$type" == "LIVE" ]; then
            if [ "$died_count" -gt 0 ]; then
                echo -e "${RED}[FAIL] A philosopher died.${NC}"
                return 1
            else
                if [ "$mode" == "SERVINGS" ]; then
                    # Verify Eat Count
                    local eat_count=$(grep "is eating" "$LOG_FILE" | wc -l)
                    local expected=$((num_philo * SERVINGS))
                    
                    if [ "$eat_count" -ge "$expected" ]; then
                        echo -e "${GREEN}[OK] (Ate: $eat_count/$expected)${NC}"
                    else
                        echo -e "${RED}[FAIL] Not enough meals. Expected $expected, got $eat_count.${NC}"
                        return 1
                    fi
                else
                    # Infinite Mode (Live)
                    echo -e "${GREEN}[OK] Survived $DURATION seconds.${NC}"
                fi
            fi
        
        elif [ "$type" == "DIE" ]; then
            if [ "$died_count" -gt 0 ]; then
                echo -e "${GREEN}[OK] A philosopher died.${NC}"
            else
                echo -e "${RED}[FAIL] No one died.${NC}"
                return 1
            fi
        fi
    done
    return 0
}

# --- MAIN EXECUTION ---

echo "========================================"
echo "    STARTING PHILOSOPHERS TESTER"
echo "========================================"

# 1. Run "SHOULD LIVE" Tests
echo -e "\n${BLUE}--- CATEGORY: SHOULD LIVE ---${NC}"
for test_args in "${TESTS_SHOULD_LIVE[@]}"; do
    if [ -n "$SERVINGS" ]; then
        run_test "$test_args" "LIVE" "SERVINGS"
    else
        run_test "$test_args" "LIVE" "INFINITE"
    fi
done

# 2. Run "SHOULD DIE" Tests
echo -e "\n${BLUE}--- CATEGORY: SHOULD DIE ---${NC}"
for test_args in "${TESTS_SHOULD_DIE[@]}"; do
    # Note: Even in serving mode, if they should die, we usually ignore the eating count 
    # and just check for death.
    if [ -n "$SERVINGS" ]; then
         run_test "$test_args" "DIE" "SERVINGS"
    else
         run_test "$test_args" "DIE" "INFINITE"
    fi
done

# Cleanup
rm -f "$LOG_FILE"
echo -e "\n========================================"
echo "    TESTING COMPLETE"
echo "========================================"