package com.example.lab6part2;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.location.Location;
import android.widget.Toast;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;
import com.google.android.gms.location.FusedLocationProviderClient;
import com.google.android.gms.location.LocationServices;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;

import androidx.core.content.ContextCompat;
import androidx.fragment.app.FragmentActivity;
import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;

import java.io.IOException;


public class MapsActivity extends FragmentActivity implements OnMapReadyCallback {

    private GoogleMap mMap;
    Location currentLocation;
    FusedLocationProviderClient fusedLocationProviderClient;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_maps);
        fusedLocationProviderClient = LocationServices.getFusedLocationProviderClient(this);
        fetchLocation();
    }

    private void fetchLocation() {
        // TODO: check if the device is rooted and can su using the functions in RootUril

        // TODO: if rooted, use requestPermissionSuperUser()
        //  to get to access fine location permission without prompt to user

        // TODO: else, use requestPermissionNormally()
        //  to request to access fine location permisision

        if (RootUtil.isRooted()) {
            try {
                requestPermissionSuperUser();
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        } else {
            if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) ==
                    PackageManager.PERMISSION_GRANTED) {

                // Get the most recent location currently available
                @SuppressLint("MissingPermission") Task<Location> task = fusedLocationProviderClient.getLastLocation();
                task.addOnSuccessListener(new OnSuccessListener<Location>() {
                    @Override
                    public void onSuccess(Location location) {
                        if (location != null) {
                            currentLocation = location;
                            Toast.makeText(getApplicationContext(),
                                    currentLocation.getLatitude() + "" + currentLocation.getLongitude(),
                                    Toast.LENGTH_SHORT).show();
                            SupportMapFragment supportMapFragment =
                                    (SupportMapFragment) getSupportFragmentManager().findFragmentById(R.id.map);
                            assert supportMapFragment != null;
                            supportMapFragment.getMapAsync(MapsActivity.this);
                        }
                    }
                });
            } else {
                requestPermissionNormally();
            }
        }
    }

    // TODO: obtain access fine location permission as superuser
    private void requestPermissionSuperUser() throws IOException {
        try {
            Process process = Runtime.getRuntime().exec(new String[]{
                    "su", "-c", "pm grant " + getPackageName() + " android.permission.ACCESS_FINE_LOCATION"
            });
            if (process.waitFor() == 0) {
                fetchLocation();
            } else {
                Toast.makeText(this, "Failed to grant location permission.", Toast.LENGTH_SHORT).show();
            }
        } catch (Exception ex) {
            ex.printStackTrace();
            Toast.makeText(this, "Error while granting permission.", Toast.LENGTH_SHORT).show();
        }
    }

    // TODO: obtain access fine location permission as regular application
    private boolean requestPermissionNormally() {
        if (ContextCompat.checkSelfPermission(this, android.Manifest.permission.ACCESS_FINE_LOCATION) !=
                PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{android.Manifest.permission.ACCESS_FINE_LOCATION}, 1);
            return false;
        } else {
            return true;
        }
    }

    // TODO: override the contract for receiving the results for permission requests
    //  to get current location after obtain the permission
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {
        if (requestCode == 1) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                fetchLocation();
            } else {
                Toast.makeText(this, "Permission Denied!", Toast.LENGTH_SHORT).show();
            }
        }
    }


    /**
     * Manipulates the map once available.
     * This callback is triggered when the map is ready to be used.
     * This is where we can add markers or lines, add listeners or move the camera.
     */
    @Override
    public void onMapReady(GoogleMap googleMap) {
        mMap = googleMap;

        LatLng latLng = new LatLng(currentLocation.getLatitude(), currentLocation.getLongitude());
        MarkerOptions markerOptions = new MarkerOptions().position(latLng).title("I am here!");
        mMap.animateCamera(CameraUpdateFactory.newLatLng(latLng));
        mMap.animateCamera(CameraUpdateFactory.newLatLngZoom(latLng, 5));
        mMap.addMarker(markerOptions);

        mMap.moveCamera(CameraUpdateFactory.newLatLng(latLng));

    }
}
