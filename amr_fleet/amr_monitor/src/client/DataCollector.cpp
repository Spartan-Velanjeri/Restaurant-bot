//
// Created by jakub on 21. 3. 2020.
//

#include <amr_monitor/client/DataCollector.h>
#include <tf/transform_datatypes.h>

DataCollector::DataCollector() {

    ros::NodeHandle nh;
    robotPoseSub = nh.subscribe("amcl_pose", 5, &DataCollector::robotPoseCb, this);
    robotCurrentGoalSub = nh.subscribe("pose_controller/current_goal", 5, &DataCollector::robotCurrentGoalCb, this);
    currentTaskSub= nh.subscribe("task_manager_client/current_task", 5, &DataCollector::currentTaskCb, this);
    clientStatusPub = nh.advertise<amr_msgs::ClientInfo>("/client_info", 5, true);

    ros::NodeHandle nhParam("~");
    nhParam.getParam("tf_prefix", tfPrefix);

    clientInfo.clientId = ros::this_node::getNamespace();

    // remove slash from client namespace
    if (!clientInfo.clientId.empty() && clientInfo.clientId[0] == '/') {
        clientInfo.clientId.erase(0, 1);
    }

    ros::Rate r(2);
    while (ros::ok()) {
        ros::spinOnce();
        if (readRobotPoseTf()) {
            clientStatusPub.publish(clientInfo);
        }
        r.sleep();
    }
}

void DataCollector::robotPoseCb(const geometry_msgs::PoseWithCovarianceStampedConstPtr &pose) {
    clientInfo.poseWithCovariance.pose.covariance = pose->pose.covariance;
}

void DataCollector::robotCurrentGoalCb(const amr_msgs::PointConstPtr &point) {
    clientInfo.robotCurrentGoal = *point;
}

void DataCollector::currentTaskCb(const amr_msgs::TaskConstPtr &task) {
    clientInfo.currentTask = *task;
    clientInfo.currentTask.waypoints = {};  // send task without waypoints
}

bool DataCollector::readRobotPoseTf() {
    tf::StampedTransform transform;
    try{
        poseTfListener.waitForTransform("/map", tf::resolve(tfPrefix, "base_link"), ros::Time::now(), ros::Duration(0.5));
        poseTfListener.lookupTransform("/map", tf::resolve(tfPrefix, "base_link"), ros::Time(0), transform);

        geometry_msgs::Pose pose;
        pose.position.x = transform.getOrigin().x();
        pose.position.y = transform.getOrigin().y();
        pose.position.z = transform.getOrigin().z();

        pose.orientation.x = transform.getRotation().x();
        pose.orientation.y = transform.getRotation().y();
        pose.orientation.z = transform.getRotation().z();
        pose.orientation.w = transform.getRotation().w();

        clientInfo.poseWithCovariance.pose.pose = pose;
        return true;
    } catch (const tf::TransformException& ex) {
        ROS_WARN("Unable to read robot pose TF, not ready yet");
        return false;
    }
}






//void DataCollector::updateRobotPose() {
//    tf::StampedTransform transform;
//    try{
//        poseTfListener.waitForTransform("/map", tf::resolve(tfPrefix, "base_link"), ros::Time::now(), ros::Duration(5.0));
//        poseTfListener.lookupTransform("/map", tf::resolve(tfPrefix, "base_link"), ros::Time(0), transform);
//        geometry_msgs::Pose2D currentRobotPose;
//        currentRobotPose.x = transform.getOrigin().x();
//        currentRobotPose.y = transform.getOrigin().y();
//        currentRobotPose.theta = tf::getYaw(transform.getRotation());
//
//        controller->setCurrentPose(currentRobotPose);
//        robotPoseReceived = true;
////        ROS_INFO_STREAM("X: " << currentRobotPose.x << " Y: " << currentRobotPose.y << " Yaw: " << currentRobotPose.theta);
//    } catch (const tf::TransformException& ex) {
//        ROS_ERROR("%s", ex.what());
//        ros::Duration(1.0).sleep();
//    }
//}
