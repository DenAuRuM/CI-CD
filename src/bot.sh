#!/bin/bash

TG_USER_ID="594751937"
TG_BOT_TOKEN="7113510234:AAEv7-UGLVxVdRTF-0liD7UMlxksjLlvyXQ"
URL="https://api.telegram.org/bot$TG_BOT_TOKEN/sendMessage"
TIME="15"

send_message() {
  message=$1
  curl -s --max-time $TIME -d "chat_id=$TG_USER_ID&disable_web_page_preview=1&text=$message" $URL > /dev/null
}

# Статус задачи
generate_job_status_message() {
  job_name=$1
  job_status=$2
  text="ℹ️ Job $job_name is in status:%0A%0A"

  if [ "$job_status" == "success" ]; then
    text+="✅ == SUCCESS == ✅%0A%0A"
  elif [ "$job_status" == "failed" ]; then
    text+="❌ == FAILED == ❌%0A%0A"
  else
    text+="👀 ==$job_status== 👀%0A%0A"
  fi

  text+="Project: $CI_PROJECT_NAME%0A \
URL: $CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/%0A \
Branch: $CI_COMMIT_REF_SLUG%0A \
Commit name: $CI_COMMIT_TITLE%0A \
ID pipeline: $CI_PIPELINE_ID%0A"
  echo "$text"
}

# Отправка сообщения о старте пайплайна
if [ "$CI_JOB_NAME" == "style-test-job" ]; then
  START_TEXT="🚀-= PIPELINE IS STARTING =-🚀%0A%0A"
  send_message "$START_TEXT"
fi

# Отправка сообщения о статусе задачи
JOB_STATUS_TEXT=$(generate_job_status_message "$CI_JOB_NAME" "$CI_JOB_STATUS")
send_message "$JOB_STATUS_TEXT"

# Отправка отдельного сообщения о завершении пайплайна
if [ "$CI_JOB_NAME" == "deploy-job" ] && [ "$CI_JOB_STATUS" == "success" ]; then
  END_TEXT="✅ == PIPELINE ENDED JOB SUCCESSFULLY == ✅%0A%0A"
  send_message "$END_TEXT"
elif [ "$CI_JOB_STATUS" == "failed" ]; then
  END_TEXT="❌ == PIPELINE IS FAILED == ❌%0A%0A"
  send_message "$END_TEXT"
fi