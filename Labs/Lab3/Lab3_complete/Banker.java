package lab3;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.lang.reflect.Array;
import java.util.Arrays;

// package Week3;

public class Banker {
    private int numberOfCustomers;    // the number of customers
    private int numberOfResources;    // the number of resources

    private int[] available;    // the available amount of each resource
    private int[][] maximum;    // the maximum demand of each customer
    private int[][] allocation;    // the amount currently allocated
    private int[][] need;        // the remaining needs of each customer

    /**
     * Constructor for the Banker class.
     *
     * @param resources         An array of the available count for each resource.
     * @param numberOfCustomers The number of customers.
     */
    public Banker(int[] resources, int numberOfCustomers) {
        // set the number of resources
        this.numberOfResources = resources.length;
        // set the number of customers
        this.numberOfCustomers = numberOfCustomers;

        // set the value of bank resources to available
        this.available = resources;

        // set the array size for maximum, allocation, and need
        this.maximum = new int[numberOfCustomers][numberOfResources];
        this.allocation = new int[numberOfCustomers][numberOfResources];
        this.need = new int[numberOfCustomers][numberOfResources];
    }

    /**
     * Sets the maximum number of demand of each resource for a customer.
     *
     * @param customerIndex The customer's index (0-indexed).
     * @param maximumDemand An array of the maximum demanded count for each resource.
     */
    public void setMaximumDemand(int customerIndex, int[] maximumDemand) {
        // add customer, update maximum and need
        maximum[customerIndex] = maximumDemand;
        for (int i = 0; i < numberOfCustomers; i++) {
            for (int j = 0; j < numberOfResources; j++) {
                need[i][j] = maximum[i][j] - allocation[i][j];
            }
        }
    }

    /**
     * Prints the current state of the bank.
     */
    public void printState() {
        System.out.println("\nCurrent state:");
        // print available
        System.out.println("Available:");
        System.out.println(Arrays.toString(available));
        System.out.println("");

        // print maximum
        System.out.println("Maximum:");
        for (int[] aMaximum : maximum) {
            System.out.println(Arrays.toString(aMaximum));
        }
        System.out.println("");
        // print allocation
        System.out.println("Allocation:");
        for (int[] anAllocation : allocation) {
            System.out.println(Arrays.toString(anAllocation));
        }
        System.out.println("");
        // print need
        System.out.println("Need:");
        for (int[] aNeed : need) {
            System.out.println(Arrays.toString(aNeed));
        }
        System.out.println("");
    }

    /**
     * Requests resources for a customer loan.
     * If the request leave the bank in a safe state, it is carried out.
     *
     * @param customerIndex The customer's index (0-indexed).
     * @param request       An array of the requested count for each resource.
     * @return true if the requested resources can be loaned, else false.
     */
    public synchronized boolean requestResources(int customerIndex, int[] request) {
        // print the request
        System.out.println("Customer " + customerIndex + " requesting");
        System.out.println(Arrays.toString(request));
        // check if request larger than need
        for (int i = 0; i < numberOfResources; i++) {
            if (request[i] > need[customerIndex][i]) {
                return false;
            }
            // check if request larger than available
            if (request[i] > available[i]) {
                return false;
            }
        }
        // check if the state is safe or not
        if (checkSafe(customerIndex, request)) {
            // if it is safe, allocate the resources to customer customerNumber
            for (int j = 0; j < numberOfResources; j++) {
                available[j] = available[j] - request[j];
                allocation[customerIndex][j] = allocation[customerIndex][j] + request[j];
                need[customerIndex][j] = need[customerIndex][j] - request[j];
            }
            return true;
        } else return false;
    }


	/**
	 * Releases resources borrowed by a customer. Assume release is valid for simplicity.
	 * @param customerIndex  The customer's index (0-indexed).
	 * @param release        An array of the release count for each resource.
	 */
	public synchronized void releaseResources(int customerIndex, int[] release) {
		// print the release
        System.out.println("Customer " + customerIndex + " releasing");
        System.out.println(Arrays.toString(release));

        // release the resources from customer customerNumber
        for (int i = 0; i < numberOfResources; i++) {   //undo what u did during allocation
            available[i] += release[i];
            allocation[customerIndex][i] -= release[i];
            need[customerIndex][i] += release[i];
        }

	}
//----------------------
	/**
	 * Checks if the request will leave the bank in a safe state.
	 * @param customerIndex  The customer's index (0-indexed).
	 * @param request        An array of the requested count for each resource.
	 * @return true if the requested resources will leave the bank in a
	 *         safe state, else false
	 */
	private synchronized boolean checkSafe(int customerIndex, int[] request) {
		// check if the state is safe

        int[] temp_avail = new int[numberOfResources];
        int[][] temp_need = new int[numberOfCustomers][numberOfResources];
        int[][] temp_allocation = new int[numberOfCustomers][numberOfResources];
        int[] work = new int[numberOfResources];
        boolean[] finish = new boolean[numberOfCustomers];
        boolean possible;


        for (int i = 0; i < this.numberOfResources; i++) {
            temp_avail[i] = available[i] - request[i];

            for (int j=0; j<this.numberOfCustomers; j++){
                if (j == customerIndex){
                    temp_need[j][i] = need[j][i] - request[i];
                    temp_allocation[j][i] = allocation[j][i] + request[i];
                }
                else {
                    temp_need[j][i] = need[j][i];
                    temp_allocation[j][i] = this.allocation[j][i];
                }
            }
            work[i] = temp_avail[i];
        }

        // finish(all) = false
        for (int i = 0; i < numberOfCustomers; i++) {
            finish[i] = false;
        }

        possible = true;

        while (possible) {
            possible = false;
            for (int i = 0; i < numberOfCustomers; i++) {
                if (!finish[i] && compareArray(temp_need[i], work)) {
                    possible = true;
                    for (int j = 0; j < this.numberOfResources; j++) {
                        work[j] += temp_allocation[i][j];
                    }
                    finish[i] = true;
                }
            }
        }

        // return finish(all) == true
        for (int i = 0; i < numberOfCustomers; i++) {
            if (!finish[i]) {
//                System.out.println("it is unsafe");
                return false;
            }
        }
//        System.out.println("it is safe");
        return true;
    }

    /**
     * checks if a1 <= a2
     * @param a1 , int array
     * @param a2 , int array
     * @return true if a1 <= a2 (for all elements)
     */
    public static boolean compareArray(int[] a1, int[] a2){
	    for (int i=0; i<a1.length; i++){
	        if (a1[i] > a2[i]){
	            return false;
            }
        }
        return true;
    }


	/**
	 * Parses and runs the file simulating a series of resource request and releases.
	 * Provided for your convenience.
	 * @param filename  The name of the file.
	 */
	public static void runFile(String filename) {

		try {
			BufferedReader fileReader = new BufferedReader(new FileReader(filename));

			String line = null;
			String [] tokens = null;
			int [] resources = null;

			int n, m;

			try {
				n = Integer.parseInt(fileReader.readLine().split(",")[1]);
			} catch (Exception e) {
				System.out.println("Error parsing n on line 1.");
				fileReader.close();
				return;
			}

			try {
				m = Integer.parseInt(fileReader.readLine().split(",")[1]);
			} catch (Exception e) {
				System.out.println("Error parsing n on line 2.");
				fileReader.close();
				return;
			}

			try {
				tokens = fileReader.readLine().split(",")[1].split(" ");
				resources = new int[tokens.length];
				for (int i = 0; i < tokens.length; i++)
					resources[i] = Integer.parseInt(tokens[i]);
			} catch (Exception e) {
				System.out.println("Error parsing resources on line 3.");
				fileReader.close();
				return;
			}

			Banker theBank = new Banker(resources, n);

			int lineNumber = 4;
			while ((line = fileReader.readLine()) != null) {
				tokens = line.split(",");
				if (tokens[0].equals("c")) {
					try {
						int customerIndex = Integer.parseInt(tokens[1]);
						tokens = tokens[2].split(" ");
						resources = new int[tokens.length];
						for (int i = 0; i < tokens.length; i++)
							resources[i] = Integer.parseInt(tokens[i]);
						theBank.setMaximumDemand(customerIndex, resources);
					} catch (Exception e) {
                        System.out.println(e.toString());
						System.out.println("Error parsing resources on line "+lineNumber+".");
						fileReader.close();
						return;
					}
				} else if (tokens[0].equals("r")) {
					try {
						int customerIndex = Integer.parseInt(tokens[1]);
						tokens = tokens[2].split(" ");
						resources = new int[tokens.length];
						for (int i = 0; i < tokens.length; i++)
							resources[i] = Integer.parseInt(tokens[i]);
						theBank.requestResources(customerIndex, resources);
					} catch (Exception e) {
						System.out.println("Error parsing resources on line "+lineNumber+".");
						fileReader.close();
						return;
					}
				} else if (tokens[0].equals("f")) {
					try {
						int customerIndex = Integer.parseInt(tokens[1]);
						tokens = tokens[2].split(" ");
						resources = new int[tokens.length];
						for (int i = 0; i < tokens.length; i++)
							resources[i] = Integer.parseInt(tokens[i]);
						theBank.releaseResources(customerIndex, resources);
					} catch (Exception e) {
						System.out.println("Error parsing resources on line "+lineNumber+".");
						fileReader.close();
						return;
					}
				} else if (tokens[0].equals("p")) {
					theBank.printState();
				}
			}
			fileReader.close();
		} catch (IOException e) {
            System.out.println(e.toString());
			System.out.println("Error opening: "+filename);
		}

	}

	/**
	 * Main function
	 * @param args  The command line arguments
	 */
	public static void main(String [] args) {
		if (args.length > 0) {
			runFile(args[0]);
		}
	}

}